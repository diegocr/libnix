#include <devices/timer.h>
#include <unistd.h>

extern void dotimer_r(ULONG,ULONG,struct timeval *);

void usleep_r(unsigned int tim)
{ struct timeval tv;

  tv.tv_secs = tim / 1000000;
  tv.tv_micro = tim % 1000000;

  dotimer_r(UNIT_VBLANK,TR_ADDREQUEST,&tv);
}
