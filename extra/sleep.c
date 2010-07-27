#include <devices/timer.h>

extern void dotimer_r(ULONG,ULONG,struct timeval *);

void sleep_r(int secs)
{ struct timeval tv;

  if ((int)(tv.tv_secs=secs)>0)
  { tv.tv_micro=0;
    dotimer_r(UNIT_VBLANK,TR_ADDREQUEST,&tv);
  }
}
