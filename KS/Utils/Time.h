#ifndef TIME_H
#define TIME_H

#include <time.h>
#include <string>

namespace KS{namespace Utils{

const int ONE_SEC_IN_USEC = 1000000;

typedef unsigned long long TimerTick;

class Time
{
public:
    Time();
    Time(time_t tv_sec, long tv_usec);

    //返回是相对时间，不是系统当前时间的时刻,单位为微秒
    TimerTick tick() const;
    void setStartTick(){m_startTick=tick();}
    /** Get elapsed time in seconds.*/
    inline double time_s() const { return delta_s(m_startTick, tick()); }

    /** Get elapsed time in milliseconds.*/
    inline double time_m() const { return delta_m(m_startTick, tick()); }

    /** Get elapsed time in microseconds.*/
    inline double time_u() const { return delta_u(m_startTick, tick()); }

    /** Get elapsed time in nanoseconds.*/
    inline double time_n() const { return delta_n(m_startTick, tick()); }


    /** Get the time in seconds between timer ticks t1 and t2.*/
    inline double delta_s( TimerTick t1, TimerTick t2 ) const { return (double)(t2 - t1)/ONE_SEC_IN_USEC; }

    /** Get the time in milliseconds between timer ticks t1 and t2.*/
    inline double delta_m( TimerTick t1, TimerTick t2 ) const { return delta_s(t1,t2)*1e3; }

    /** Get the time in microseconds between timer ticks t1 and t2.*/
    inline double delta_u( TimerTick t1, TimerTick t2 ) const { return delta_s(t1,t2)*1e6; }

    /** Get the time in nanoseconds between timer ticks t1 and t2.*/
    inline double delta_n( TimerTick t1, TimerTick t2 ) const { return delta_s(t1,t2)*1e9; }


    void setSecond(time_t s) { m_sec = s; }
    /**
     * Returns <i>seconds</i> value.
     */
    time_t second() const { return m_sec; }
    /**
     * Sets the <i>microseconds</i> value.
     */
    long microSecond() const { return m_usec; }
    /**
     * Returns <i>microseconds</i> value.
     */
    void setMicroSecond(long us) { m_usec = us; }

    std::string toString();

    static Time getCurrentTime();

private:
    TimerTick m_startTick;
    time_t m_sec;  /* seconds */
    long m_usec;  /* microseconds */
};


}} //namespace KS::Utils
#endif // TIME_H
