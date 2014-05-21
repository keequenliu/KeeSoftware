#ifndef TSPACKET_H
#define TSPACKET_H

#include <stdint.h>
#include <iostream>
#include <list>
#include <vector>

namespace KS{ namespace TS{

#define TS_PACKET_SIZE      188
#define TS_HEADER_SIZE      4
#define TS_HEADER_SIZE_AF   6
#define TS_HEADER_SIZE_PCR  12

#define TS_SYNC_BYTE 0x47

class PIDList
{
public:

    bool contain(int pid)
    {
        std::list<int>::iterator ir=m_list.begin();
        while(ir!= m_list.end())
        {
            if((*ir) ==pid)
            {
                return true;
            }
            ir++;
        }

        return false;
    }

    void append(int pid)
    {
        if(!contain(pid))
        {
            m_list.push_back(pid);
        }
    }


private:
    std::list<int> m_list;
};

/** PES包
  * 数据，连续性
 **/
class PesPacket
{
public:
    PesPacket()
        :m_lastCC(-1)
        ,m_pesHaveError(false)
        ,m_pts(0)
    {
        m_block.clear();
    }

    ~PesPacket()
    {
        m_block.clear();
        m_lastCC=-1;
    }

    void append(uint8_t* pes,int len,int cc)
    {
        append( pes,len,cc,0);
    }

    void append(uint8_t* pes,int len,int cc,uint64_t pts)
    {
        if(haveError())
        {
            std::cerr<<" pes have cc error,so not add data to pes\n";
            return ;
        }

        if(m_lastCC>0)
        {
            //不是第一次了
            int expectCC=(m_lastCC + 1) & 0x0f;
            if(expectCC!=cc)
            {
                setError(true);
            }
        }

        for(int i=0;i<len;i++)
        {
            m_block.push_back(pes[i]);
        }
        m_lastCC=cc;
        m_pts=pts;
    }

    uint8_t* data()
    {
        return &m_block[0];
    }

    /** 主要验证cc error **/
    void setError(bool flag)
    {
        m_pesHaveError=flag;
    }

    bool haveError()
    {
        return m_pesHaveError;
    }

private:

    int m_lastCC;
    bool m_pesHaveError;
    uint64_t m_pts;
    std::vector<uint8_t> m_block;
};

/** pes packet list **/
class PesPacketList
{
public:
    PesPacketList()
        :m_curPesPacket(NULL)
    {}

    void append(uint8_t* pesPayload,int len,int cc,uint64_t pts,bool isStart)
    {
        if(isStart)
        {
            if(m_curPesPacket!=NULL)
            {
                if(!m_curPesPacket->haveError())
                    m_pktList.push_back(m_curPesPacket);
                else
                {
                    delete m_curPesPacket;
                    m_curPesPacket=NULL;
                }
            }
            m_curPesPacket=new PesPacket();
            m_curPesPacket->append(pesPayload,len,cc,pts);
        }
        else
        {
            if(m_curPesPacket==NULL)
            {
                std::cout<<" pespacket not create ,but is have data,so drop current data\n";
                return;
            }
            m_curPesPacket->append(pesPayload,len,cc);
        }
    }
private:
    std::list<PesPacket*> m_pktList;
    PesPacket* m_curPesPacket;
};


class TSPacket
{
public:
    TSPacket();

    /** 生成TS包 ,根据生成包的类型(PAT,PMT,PES，PCR)，先生成对应类型包的相关头部信息，从而计算出负载的大小
     * ，最后复制负载大小的部进入包。同时把包入队管理。
    **/
    //    uint8_t* generatePacket();
    //    void addPayloadInfo(uint8_t* payload,int len);

    /** 解析TS包 **/
    bool parseProcess(uint8_t* data,int len);
    bool checkValid();
    //解析成功后，获取负载数据，主要为PES数据。
    uint8_t* getPayloadInfo();
    bool parseHeader();
    void parsePAT(uint8_t* buf,int len);
    void parsePMT(uint8_t* buf,int len);
    void parsePES(uint8_t* buf,int len);

private:
    void initTSHeader();
    /** TS 包头信息 **/
    uint8_t m_syncByte;
    int m_transportError;
    int m_payloadStartIndicator;
    int m_transportPriority;
    uint16_t m_pid;
    int m_transportScramblingControl;
    int m_adaptationFieldControl;
    int m_continuityCounter;
    int m_last_cc;

    bool m_havePayload;
    bool m_haveAdaptation;
    /** adaptation field **/
    bool m_af_DiscontinutityIndicator;
    bool m_af_RAccessInicator;
    bool m_af_ESPriority;
    bool m_af_PCR; //1 means adaptation field does contain a PCR field
    bool m_af_OPCR; // 	1 means adaptation field does contain an OPCR field
    bool m_af_SplicePointFlag; // 	1 means presence of splice countdown field in adaptation field
    bool m_af_PrivateDataFlag;
    bool m_af_ExtensionFlag;


    /** 数据 **/
    //TS包对应的数据指针
    uint8_t* m_packetInfo;
    //传入的数据指针
    uint8_t* m_data;
    //传入数据的长度。
    int m_dataLen;
    int m_payloadStartPos;

    //pat中包含的pmt id
    PIDList m_PMTList;
    //pes的pid，暂时只记录视频的pid，音频暂不处理。
    PIDList m_PESList;
    PesPacketList m_pesPacketList;

    void printHeader();

};


}} //namespace

#endif // TSPACKET_H
