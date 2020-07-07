#ifndef TIME_LINUX_OS_H
#define TIME_LINUX_OS_H

#include <sys/time.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>
#include <string.h>

inline unsigned long long timeGetTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec*1000 + now.tv_usec/1000;
}

typedef union _LARGE_INTEGER {
  struct {
    uint32_t LowPart;
    int32_t  HighPart;
  } ;
  struct {
    uint32_t LowPart;
    int32_t  HighPart;
  } u;
  int64_t QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

/* Helpful conversion constants. */
static const unsigned usec_per_sec = 1000000;
static const unsigned usec_per_msec = 1000;

/* These functions are written to match the win32
   signatures and behavior as closely as possible.
*/
inline bool QueryPerformanceFrequency(LARGE_INTEGER *frequency)
{
    /* Sanity check. */
    assert(frequency != NULL);

    /* gettimeofday reports to microsecond accuracy. */
    frequency->QuadPart = usec_per_sec;

    return true;
}

inline bool QueryPerformanceCounter(LARGE_INTEGER *performance_count)
{
    struct timeval time;

    /* Sanity check. */
    assert(performance_count != NULL);

    /* Grab the current time. */
    gettimeofday(&time, NULL);
    performance_count->QuadPart = time.tv_usec + /* Microseconds. */
                         time.tv_sec * usec_per_sec; /* Seconds. */

    return true;
}

inline timer_t timeSetEvent(unsigned int uDelay,unsigned int uResolution,void(*TimerCallBack)(sigval_t v),void *lpParam)
{
    timer_t tid;
    struct sigevent se;
    struct itimerspec ts,ots;
    memset(&se,0,sizeof(se));
    memset(&ts,0,sizeof(ts));
    memset(&ots,0,sizeof(ots));
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = TimerCallBack;
    se.sigev_value.sival_ptr = lpParam;
    se.sigev_notify_attributes = NULL;
    if(timer_create(CLOCK_REALTIME,&se,&tid) < 0)
    {
        //perror("timer_create err");
        return NULL;
    }

    ts.it_value.tv_sec = uDelay/1000; ///ms to s
    ts.it_value.tv_nsec = (uDelay%1000)*1000*1000; ///ms to ns
    ts.it_interval.tv_sec = uResolution/1000; ///ms to s
    ts.it_interval.tv_nsec = (uResolution%1000)*1000*1000; ///ms to ns

    if(timer_settime(tid,0/*TIMER_ABSTIME*/,&ts,&ots) < 0) ///0 relative time
    {
        //perror( "timer_settime err");
        timer_delete(tid);
        return NULL;
    }
    return tid;
}

inline void timeKillEvent(timer_t timer)
{
	if(timer)
        timer_delete(timer);
}

inline int CreateTimer(unsigned int uDelay)
{
   struct itimerspec new_value;
   struct timespec now;
	unsigned int uInitExp=1000;
	
   if (clock_gettime(CLOCK_REALTIME, &now) == -1)
   {
	  //perror("clock_gettime");
	  return -1;
   }

   /* Create a CLOCK_REALTIME absolute timer with initial
	  expiration and interval as specified in command line */
	  	
   new_value.it_value.tv_sec = now.tv_sec + uInitExp/1000;
   new_value.it_value.tv_nsec = now.tv_nsec + (uInitExp%1000)*1000*1000;

   new_value.it_interval.tv_sec = uDelay/1000;
   new_value.it_interval.tv_nsec = (uDelay%1000)*1000*1000;

   int  fd= timerfd_create(CLOCK_REALTIME, 0);
   if (fd == -1)
   {
	   //perror("timerfd_create");
   	   return -2;
   }

   if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
   {
	   //perror("timerfd_settime");
	   return -3;
   }

   return fd;
}


#endif // TIME_LINUX_OS_H