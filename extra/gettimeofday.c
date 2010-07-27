#include <sys/time.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/timer.h>

extern long __gmtoffset;

int gettimeofday(struct timeval *tv, struct timezone *tzp)
{
  if (tv) {
    #if 0 // libnix < 3.x
    struct DateStamp t;
    DateStamp(&t); /* Get timestamp */
    tv->tv_sec=((t.ds_Days+2922)*1440+t.ds_Minute+__gmtoffset)*60+
               t.ds_Tick/TICKS_PER_SECOND;
    tv->tv_usec=(t.ds_Tick%TICKS_PER_SECOND)*1000000/TICKS_PER_SECOND;
    #else
    GetSysTime(tv);
    tv->tv_sec += (252460800 + (60*__gmtoffset));
    #endif
  }
  if (tzp) {
    tzp->tz_minuteswest = __gmtoffset;
    tzp->tz_dsttime = -1;
  }

  return 0;
}

/*
 * 2922 is the number of days between 1.1.1970 and 1.1.1978 (2 leap years and 6 normal)
 * 1440 is the number of minutes per day
 *   60 is the number of seconds per minute
 */
