#include "Time.h"

//#include <sys/types.h>
//#include <sys/time.h>
//#include <sys/timeb.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>

namespace KS{namespace Utils{

Time::Time()
{
}

Time::Time(time_t tv_sec_, long tv_usec_)
    : m_sec(tv_sec_)
    , m_usec(tv_usec_)
{
    //    assert (m_usec < ONE_SEC_IN_USEC);
}

TimerTick Time::tick() const
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((TimerTick)ts.tv_sec)*1000000+(TimerTick)ts.tv_nsec/1000;
}

Time Time::getCurrentTime()
{
#if 1 //POSIX_TIME
    //linux clock get time
    struct timespec ts;

    //CLOCK_REALTIME 表示系统的当前时间,绝对时间,ts中得到的是相对1970年1月1日0时0分0秒 (UTC)的秒数
    //CLOCK_MONOTONIC 表示与1970年1月1日0时0分0秒 相减的时间 相对时间
    int res = clock_gettime (CLOCK_REALTIME, &ts);

    return Time (ts.tv_sec, ts.tv_nsec / 1000);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return Time (tv.tv_sec, tv.tv_usec);
#endif
}

std::string Time::toString()
{
    time_t tick=this->m_sec+this->m_usec/(ONE_SEC_IN_USEC*1000);
    tm* now=localtime(&tick);

    char strTime[256]={0};
    sprintf(strTime,"%04d/%02d/%02d %02d:%02d:%02d",(now->tm_year+1900),(now->tm_mon+1),now->tm_mday
            ,now->tm_hour,now->tm_min,now->tm_sec);

    std::string str(strTime);

    return str;
}

}}
