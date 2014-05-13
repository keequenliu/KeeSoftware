#include "Log.h"

#include <sstream>

namespace KS{ namespace LOG {

//class NullStreamBuffer : public std::streambuf
//{
//private:
//    std::streamsize xsputn(const std::streambuf::char_type *str, std::streamsize n)
//    {
//        return n;
//    }
//};

//struct NullStream : public std::ostream
//{
//public:
//    NullStream():
//        std::ostream(new NullStreamBuffer)
//    { _buffer = dynamic_cast<NullStreamBuffer *>(rdbuf()); }

//    ~NullStream()
//    {
//        rdbuf(0);
//        delete _buffer;
//    }

//protected:
//    NullStreamBuffer* _buffer;
//};


static LogLevel g_LogLevel = NOTICE;
//static NullStream g_NullStream;
static bool s_LogNeedInited = true;
static std::ofstream g_file;
//////c++ log



bool initLogLevel()
{

    g_file.open("log.txt");


    char* OSGNOTIFYLEVEL=getenv("OSG_NOTIFY_LEVEL");
    s_LogNeedInited=false;
}

//bool isLogEnabled(LogLevel severity)
//{
//    if (s_LogNeedInited) initLogLevel();
//    return severity<=g_LogLevel;
//}

std::ostream& log(/*LogLevel severity*/)
{
    std::cerr<<"test\n";
    if (s_LogNeedInited) initLogLevel();

    //    if (isLogEnabled((LogLevel)severity))
    //    {
    //        return g_file;
    //    }
    return g_file;
}

void test()
{
    std::cerr<<"hello test :"<<__FILE__<<std::endl;

}



//c
Logger::Logger()
{
    m_inited=false;
    m_appName="app";
    m_path=".";
}

Logger::~Logger()
{
    close();
}

void Logger::config(std::string appName,std::string path)
{
    m_appName=appName;
    m_path=path;
}

void Logger::init()
{
    char hostName[256]={0};
    gethostname(hostName,sizeof(hostName));
    std::stringstream ss;
    ss<<m_path<<"//"<<hostName<<"-"<<m_appName<<".log";
    m_fileStream.open(ss.str().c_str());
    m_inited=true;
}

void Logger::initLogLevel(LogLevel level)
{
    m_logLevel=level;
}

bool Logger::checkLogLevel(const LogLevel level)
{
    return (level<=m_logLevel);
}

void Logger::close()
{
    m_fileStream.flush();
    m_fileStream.close();
}



}}
