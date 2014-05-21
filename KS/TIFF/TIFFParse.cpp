#include "TIFFParse.h"

namespace KS{namespace Tiff{

TIFFParse::TIFFParse()
    :m_tiff(NULL)
{
}

TIFFParse::~TIFFParse()
{
    if(m_tiff)
    {
        TIFFClose(m_tiff);
    }
}

void TIFFParse::open(std::string fileName)
{
    m_tiff=TIFFOpen(fileName.c_str(),"r");

    if(m_tiff)
    {
        std::cout<<"tiff file open success \n";
    }


    //parse dir info
    int dirSize=TIFFNumberOfDirectories(m_tiff);
    std::cout<<"dir count :"<<dirSize<<std::endl;

    //    int dirCount=0;
    //    while(TIFFReadDirectory(m_tiff))
    //    {
    //        dirCount++;
    //    }
    //    std::cout<<"tiff file have dir count :"<<dirCount<<std::endl;

    int tiffImageWidth,tiffImageHeight,tiffImageSize;
//    TIFFGetField(m_tiff,TIFFTAG_IMAGEWIDTH,&tiffImageWidth);
//    TIFFGetField(m_tiff,TIFFTAG_IMAGELENGTH,&tiffImageHeight);

    int tileWidth,tileHeight;
//    TIFFGetField(m_tiff,TIFFTAG_TILEWIDTH,&tileWidth);
//    TIFFGetField(m_tiff,TIFFTAG_TILELENGTH,&tileHeight);

//    std::cout<<"tile width: "<<tileWidth<<", tile height: "<<tileHeight<<std::endl;

//    int tiffImageSize=tiffImageWidth*tiffImageHeight;

//    std::cout<<"tiff image size :"<<tiffImageSize<<",W : "<<tiffImageWidth<<", H: "<<tiffImageHeight<<std::endl;


    tileWidth=tileHeight=tiffImageWidth=tiffImageHeight=tiffImageSize=0;
    //
    for(int i=0;i<dirSize;i++)
    {
        TIFFSetDirectory(m_tiff,i);

        char tileName[1024];
        TIFFGetField(m_tiff,TIFFTAG_PAGENAME,&tileName);
        std::cout<<" dir index :"<<i<<", name :"<<tileName<<std::endl;

        TIFFGetField(m_tiff,TIFFTAG_IMAGEWIDTH,&tiffImageWidth);
        TIFFGetField(m_tiff,TIFFTAG_IMAGELENGTH,&tiffImageHeight);
        std::cout<<"\ttiff image size :"<<tiffImageSize<<",W : "<<tiffImageWidth<<", H: "<<tiffImageHeight<<std::endl;

        TIFFGetField(m_tiff,TIFFTAG_TILEWIDTH,&tileWidth);
        TIFFGetField(m_tiff,TIFFTAG_TILELENGTH,&tileHeight);
        int size=TIFFTileSize(m_tiff);

        std::cout<<"\ttile width: "<<tileWidth<<", tile height: "<<tileHeight<<std::endl;
        std::cout<<"\ttile size :"<<size<<std::endl;

//        uint16 simplePixel,simpleBit;
//        TIFFGetField(m_tiff,TIFFTAG_SAMPLESPERPIXEL,&simplePixel);
//        TIFFGetField(m_tiff,TIFFTAG_BITSPERSAMPLE,&simpleBit);
//        uint16 pixelBits=simplePixel * simpleBit;
//        int image

    }
}

}} //namespace
