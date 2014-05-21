#include "TSPacket.h"

#include <assert.h>
#include <cstring>
#include "PSI.h"

namespace KS{ namespace TS{

TSPacket::TSPacket()
    :m_data(NULL)
    ,m_dataLen(0)
    ,m_packetInfo(NULL)
    ,m_payloadStartPos(0)
{
    //    m_PMTList();
    //    m_PESList.clear();
}


/** 解析TS包 **/

bool TSPacket::checkValid()
{
    bool ret=false;
    int last_sync_pos=-1;
    int offset=0;
    int packet_size=0;
    m_packetInfo=m_data;
    while((m_packetInfo=(uint8_t*)memchr(m_packetInfo,TS_SYNC_BYTE,m_dataLen))!=NULL)
    {
        offset=m_packetInfo-m_data;
        if(last_sync_pos==-1)
        {
            last_sync_pos=offset;
        }
        packet_size=offset-last_sync_pos;
        if(packet_size == TS_PACKET_SIZE)
        {//这里只考虑188大小的TS包
            return true;
        }

        m_packetInfo++;
    }

    return ret;
}

bool TSPacket::parseProcess(uint8_t* data,int len)
{
    assert(data != NULL);
    //    std::cerr<<"len: "<<len<<std::endl;
    m_data=data;
    m_dataLen=len;
    if(checkValid())
    {
        m_packetInfo=m_packetInfo-TS_PACKET_SIZE; //从第一个0x47开始
        //        m_packetInfo=m_data;
        //找到0x47
        int offset=m_packetInfo-m_data;
        uint8_t* dataEnd=m_data+m_dataLen;

        while((m_packetInfo+TS_PACKET_SIZE) <= dataEnd)
        {
            //            for(int i=0;i<188;i++)
            //            {
            //                std::cerr<<std::hex<<std::showbase<<(int)m_packetInfo[i]<<" ";

            //                if((i+1) % 16 ==0)
            //                    std::cerr<<std::endl;

            //            }
            if(parseHeader())
            {
                if(m_havePayload)
                {
                    if(m_pid==0)
                    {
                        if(m_payloadStartIndicator)
                        {
                            //skip pointfield
                            m_payloadStartPos+=m_packetInfo[m_payloadStartPos]+1;
                        }
                        parsePAT(m_packetInfo+m_payloadStartPos,TS_PACKET_SIZE-m_payloadStartPos);
                    }
                    else if(m_pid==0x01 || m_pid==0x02 || m_pid==0x03)
                    {
                        std::cerr<<"current ts packet is  CAT or TSDT or IPMP \n";
                    }
                    else if(m_PMTList.contain(m_pid))
                    {
                        if(m_payloadStartIndicator)
                        {
                            m_payloadStartPos+=m_packetInfo[m_payloadStartPos]+1;
                        }
                        parsePMT(m_packetInfo+m_payloadStartPos,TS_PACKET_SIZE-m_payloadStartPos);
                    }
                    else if(m_PESList.contain(m_pid))
                    {
                        parsePES(m_packetInfo+m_payloadStartPos,TS_PACKET_SIZE-m_payloadStartPos);
                    }
                }
                else
                {
                    std::cerr<<"it is a null ts packet\n";
                }
            }
            else
            {
                return false;
            }

            m_packetInfo+=TS_PACKET_SIZE;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void TSPacket::initTSHeader()
{
    m_syncByte=-1;
    m_transportError=-1;
    m_payloadStartIndicator=-1;
    m_transportPriority=-1;
    m_pid=-1;
    m_transportScramblingControl=-1;
    m_adaptationFieldControl=-1;
    m_continuityCounter=-1;

    m_havePayload=false;
    m_haveAdaptation=false;
    /** adaptation field **/
    m_af_DiscontinutityIndicator=false;
    m_af_RAccessInicator=false;
    m_af_ESPriority=false;
    m_af_PCR=false;
    m_af_OPCR=false;
    m_af_SplicePointFlag=false;
    m_af_PrivateDataFlag=false;
    m_af_ExtensionFlag=false;
}


bool TSPacket::parseHeader()
{
    initTSHeader();
    if(m_packetInfo[0]!=TS_SYNC_BYTE)
    {
        std::cerr<<"TS Head is not 0x47\n";
        return false;
    }
    m_transportError=m_packetInfo[1]& 0x80;
    m_payloadStartIndicator= (m_packetInfo[1] & 0x40);
    m_transportPriority= (m_packetInfo[1] & 0x20);
    m_pid=((m_packetInfo[1] & 0x1f) << 8) | m_packetInfo[2];
    m_continuityCounter=m_packetInfo[3]&0x0f;
    m_transportScramblingControl=(m_packetInfo[3] & 0xc0) >> 6;

    m_havePayload=(m_packetInfo[3] & 0x10);
    m_haveAdaptation=m_packetInfo[3] & 0x20;

    m_payloadStartPos=4;
    if(m_haveAdaptation)
    {
        int adaptationFieldLen=m_packetInfo[4];
        m_payloadStartPos+=adaptationFieldLen+1;
        m_af_DiscontinutityIndicator=m_packetInfo[5] & 0x80;
        m_af_RAccessInicator=m_packetInfo[5] & 0x40;
        m_af_ESPriority=m_packetInfo[5] & 0x20;
        m_af_PCR=m_packetInfo[5] & 0x10;

        m_af_OPCR=m_packetInfo[5] & 0x08;
        m_af_SplicePointFlag=m_packetInfo[5] & 0x04;
        m_af_PrivateDataFlag=m_packetInfo[5] & 0x02;
        m_af_ExtensionFlag=m_packetInfo[5] & 0x01;

        if(m_af_PCR)
        {
            //have pcr
            uint64_t pcr=((uint64_t) m_packetInfo[6] << 25) | (m_packetInfo[7] << 17) | (m_packetInfo[8] << 9) | (m_packetInfo[9] << 1) |
                    (m_packetInfo[10] >> 7); //33bits
            uint64_t pcr_ext=((m_packetInfo[10] & 1) << 8) | m_packetInfo[11]; //9bits
            std::cerr<<"adaptationField have pcr \n";
            std::cerr<<"pcr :"<<pcr<<std::endl
                    <<"pcr_ext: "<<pcr_ext<<std::endl;
        }

        if(m_af_OPCR)
        {
            std::cerr<<"adaptationField have opcr \n";
        }
        if(m_af_SplicePointFlag)
        {
            std::cerr<<"adaptationField have SplicePoint \n";
        }
        if(m_af_DiscontinutityIndicator)
        {
            std::cerr<<"adaptationField have DiscontinutityIndicator \n";
        }
        if(m_af_ExtensionFlag)
        {
            std::cerr<<"adaptationField have ExtensionFlag \n";
        }
        if(m_af_PrivateDataFlag)
        {
            std::cerr<<"adaptationField have PrivateDataFlag \n";
        }
    }

    return true;
}

void TSPacket::parsePAT(uint8_t* patSection,int len)
{
    int tableID=patSection[0];
    if(tableID!=0x00 || len<8)
    {
        std::cerr<<"it is not a pat section \n";
        return ;
    }
    int sectionLen=(patSection[1]&0x0f)<<8 | patSection[2]; //长度表示，从它后面开始包括CRC
    int versionNumber=patSection[5]>>1 &0x1f;
    int currentNextIndicator=patSection[5] & 0x01;
    int sectionNumber=patSection[6]; //从0开始
    int lastSectionNumber=patSection[7]; //总共多少section

    /** 这里并没有用sectionNumber与lastSectionNumber来计算Program数量
      * 并不确定这两个值会正确处理，所以通过长度来判断。
     **/

    std::cerr<<"current sectionNumber: "<<sectionNumber
            <<", "<<lastSectionNumber<<std::endl;

    int offset=8;
    int programLen=sectionLen-9; //CRC(4)+sectionLen(后面有5)
    int programCount=programLen/4;  //每个program 4 Bytes= programNumber(16bit) +reserved(3bits) + pid(13bit)
    int programNumber, pmtPID;
    for(int i=0;i<programCount;i++)
    {
        programNumber = (patSection[offset++]<<8);
        programNumber |= patSection[offset++];
        if(programNumber==0)
        {
            std::cerr<<"it is a network pid,but now not process it \n";
        }
        else
        {
            pmtPID =  ((patSection[offset++]&0x1f) <<8 );
            pmtPID |=  patSection[offset++];
            std::cerr<<"\t pmt pid :"<<pmtPID<<std::endl;
            m_PMTList.append(pmtPID);
        }
    }

    //check crc
    uint32_t crc = 0xffffffff;
    uint16_t crcOffset = sectionLen + 3 - 4;
    uint16_t i;

    //从开始至crc的开始位置，即所有数据进行CRC计算
    for (i = 0; i < crcOffset; i++)
        crc = (crc << 8) ^ psi_crc_table[(crc >> 24) ^ (patSection[i])];

    bool checkCRC=patSection[crcOffset] == (crc >> 24)
            && patSection[crcOffset + 1] == ((crc >> 16) & 0xff)
            && patSection[crcOffset + 2] == ((crc >> 8) & 0xff)
            && patSection[crcOffset + 3] == (crc & 0xff);
    if(!checkCRC || (crcOffset != offset))
    {
        std::cerr<<"pat crc check error \n";
    }
}

void TSPacket::parsePMT(uint8_t* pmtSection,int len)
{
    int tableID=pmtSection[0];
    if(tableID!=0x02)
    {
        std::cerr<<"it is not a pmt table \n";
        return;
    }

    int sectionLen=(pmtSection[1]&0x0f)<<8 | pmtSection[2]; //长度 从它后面开始包括CRC
    int programNumber=(pmtSection[3]<<8) |pmtSection[4];
    int versionNumber=pmtSection[5]>>1 &0x1f;
    int currentNextIndicator=pmtSection[5] & 0x01;
    int sectionNumber=pmtSection[6]; //从0开始
    int lastSectionNumber=pmtSection[7]; //总共多少section

    //从第8个开始
    int programPCRPID=(pmtSection[8]&0x1f)<<8 |pmtSection[9];
    int programInfoLen=(pmtSection[10]&0x0f)<<8 |pmtSection[11]; //表示描述信息的长度

    int offset=12+programInfoLen; //12 从tableid开始的12个字节
    int pesLen=sectionLen+2-4; //开始2bits,最后4bits CRC

    int streamType,elementaryPID,ESInfoLen;
    while(offset<=pesLen)
    {
        streamType=pmtSection[offset++];
        elementaryPID=(pmtSection[offset++] & 0x1f) <<8;
        elementaryPID |= pmtSection[offset++];

        ESInfoLen=pmtSection[offset++]& 0x0f << 8;
        ESInfoLen |=pmtSection[offset++];


        if(streamType ==0x1b)
        {//只考虑AVC Video Stream
            m_PESList.append(elementaryPID);
        }
        else
        {
            std::cerr<<"pmt contain stream type is "<<elementaryPID<<std::endl;
        }

        offset+=ESInfoLen;
    }


    //check crc
    uint32_t crc = 0xffffffff;
    uint16_t crcOffset = sectionLen + 3 - 4;
    uint16_t i;

    //从开始至crc的开始位置，即所有数据进行CRC计算
    for (i = 0; i < crcOffset; i++)
        crc = (crc << 8) ^ psi_crc_table[(crc >> 24) ^ (pmtSection[i])];

    bool checkCRC=pmtSection[crcOffset] == (crc >> 24)
            && pmtSection[crcOffset + 1] == ((crc >> 16) & 0xff)
            && pmtSection[crcOffset + 2] == ((crc >> 8) & 0xff)
            && pmtSection[crcOffset + 3] == (crc & 0xff);
    if(!checkCRC || (crcOffset != offset))
    {
        std::cerr<<"pmt crc check error \n";
    }

}


void TSPacket::parsePES(uint8_t* pesSection,int len)
{
    if(m_payloadStartIndicator)
    {
        //pes start
        if((pesSection[0] == 0x0 && pesSection[1] == 0x0 && pesSection[2] == 0x01
            && pesSection[3] >= 0xBC))
        {
            uint8_t streamID=pesSection[3];
            int pesLen=(pesSection[4]<<8) | pesSection[5]; //pes packet len
            int dataAlignmentIndicator=(pesSection[6] & 0x4);
            bool havePTS=(pesSection[7] & 0x80);
            bool haveDTS=(pesSection[7] & 0xc0) == 0xc0;
            int pesHeaderLen=pesSection[8];
            uint64_t pts=0,dts=0;

            if((pesSection[6] & 0xc0) == 0x80)
            {//check pes header

                if(havePTS)
                {
                    pts=(((uint64_t)pesSection[9] & 0xe)) << 29 | (pesSection[10] << 22) |
                                                             ((pesSection[11] & 0xfe) << 14) | (pesSection[12] << 7) |
                                                             ((pesSection[13] & 0xfe) >> 1);
                    //这里比较对应的markbit是否正确，如果不正确 说明此包存在序列问题。
                    bool checkPTS=((pesSection[9] & 0xe1) == 0x21)
                            && (pesSection[11] & 0x1) && (pesSection[13] & 0x1);
                    if(!checkPTS)
                    {
                        pts=0;
                        std::cout<<"pts check error \n";
                    }
                    std::cerr<<"pts :"<<pts<<std::endl;
                }
                if(haveDTS)
                {
                    dts=(((uint64_t)pesSection[14] & 0xe)) << 29 | (pesSection[15] << 22) |
                                                              ((pesSection[16] & 0xfe) << 14) | (pesSection[17] << 7) |
                                                              ((pesSection[18] & 0xfe) >> 1);

                    bool checkDTS= (pesSection[9] & 0x10) && ((pesSection[14] & 0xf1) == 0x11)
                            && (pesSection[16] & 0x1) && (pesSection[18] & 0x1);
                    if(!checkDTS)
                    {
                        dts=0;
                        std::cout<<"pts check error \n";
                    }
                }

            }
            else
            {
                std::cerr<<"pes head is error,start byte is not 0x10 \n";
            }
            int offset=9+pesHeaderLen; //可选 pes 首部长度 ＋

        }
        else
        {
            std::cerr<<"pes start code prefix is not 0x00 00 01 or streamid < 0xbc \n";
            return ;
        }
        //        m_pesPacketList.append(pesSection+offset,len-offset,m_continuityCounter,m_payloadStartIndicator,pts);
    }
    else
    {
        if(m_payloadStartIndicator)
        {
            std::cerr<<" parese pes error , payloadIndicator must be false ,but it's true\n";
        }
        //        m_pesPacketList.append(pesSection,len,m_continuityCounter,m_payloadStartIndicator,0);
    }
}


}} //namespace
