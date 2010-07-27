#if 0
#include <devices/timer.h>

extern void dotimer(ULONG,ULONG,struct timeval *);

void timer(long clock[2])
{
  dotimer(UNIT_VBLANK,TR_GETSYSTIME,(struct timeval *)&clock[0]);
}
#else
#include <proto/timer.h>

void timer(long clock[2])
{
  GetSysTime((struct timeval *)&clock[0]);
}
#endif
