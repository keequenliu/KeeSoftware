#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <iosfwd>
#include <fstream>

namespace KS{namespace LOG{

enum LogLevel
{
    ALWAYS=0, //一直显示的信息，最简单的信息显示
    FATAL=1,
    WARN=2,
    NOTICE=3,
    INFO=4,
    DEBUG=5,
};

//extern void setLogLevel(LogLevel severity);

//extern bool isLogEnabled(LogLevel severity);

extern std::ostream& log(/*LogLevel severity*/);

void test();

//inline std::ostream& log(void) { return log(INFO); }

//#define KS_LOG(level) if (isLogEnabled(level)) log(level)
//#define KS_LOG(level) log(level)
//#define KS_ALWAYS KS_LOG(ALWAYS)
//#define KS_FATAL KS_LOG(FATAL)
//#define KS_WARN KS_LOG(WARN)
//#define KS_NOTICE KS_LOG(NOTICE)
//#define KS_INFO KS_LOG(INFO)
//#define KS_DEBUG KS_LOG(DEBUG)

//#define LOGMASKBIT 5
//#define LOGMASK   ((1 << LOGMASKBIT)-1)

//#define LOGSAMBA  (1 << (LOGMASKBIT+0))
//#define LOGCURL   (1 << (LOGMASKBIT+1))
//#define LOGCMYTH  (1 << (LOGMASKBIT+2))
//#define LOGFFMPEG (1 << (LOGMASKBIT+3))
//#define LOGRTMP   (1 << (LOGMASKBIT+4))
//#define LOGDBUS   (1 << (LOGMASKBIT+5))

/**
  * 能根据不同级别，来确定是否输出日志。（用环境变量来进行配置）
  * 支持格式化输出日志
  * 同时支持终端与日志文件输出
  * 支持 << 写入
  * 单例模式
  **/
class Logger
{
public:
    Logger();
    virtual ~Logger();

    void config(std::string appName,std::string path=".");
    //如果没有设置，则从环境变量读取，如果环境变量也没有，默认值为INFO
    void initLogLevel(LogLevel level);
    bool checkLogLevel(const LogLevel level);

    std::ostream& log()
    {
        if(!m_inited)
        {
            init();
        }

        return m_fileStream;
    }

//    friend std::ostream& operator<<(std::ostream& os, const T& obj)
//    {
//      // write obj to stream
//      return os;
//    }
//    friend std::istream& operator>>(std::istream& is, T& obj)
//    {
//      // read obj from stream
//      if( /* T could not be constructed */ )
//        is.setstate(std::ios::failbit);
//      return is;
//    }

private:
    void init();

    void close();

    LogLevel m_logLevel;
    std::ofstream m_fileStream;
    std::string m_appName;
    std::string m_path;
    bool m_inited;
};

}}// namespace KS::LOG

#endif // LOG_H
