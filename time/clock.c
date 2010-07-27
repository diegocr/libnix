#include <time.h>
#include <limits.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include "stabs.h"

#if 0 // libnix < 3.0x
static struct DateStamp ds;

void __initclock(void)
{ DateStamp(&ds); }

ADD2INIT(__initclock,-10);

clock_t clock(void)
{ struct DateStamp ds2;
  DateStamp(&ds2);
  return (((ds2.ds_Days-ds.ds_Days)*(24*60)+
           ds2.ds_Minute-ds.ds_Minute)*(60*TICKS_PER_SECOND)+
           ds2.ds_Tick-ds.ds_Tick)*CLOCKS_PER_SEC/TICKS_PER_SECOND;
}
#elif 0
#include <proto/exec.h>
#include <proto/timer.h>
#include <proto/dos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <SDI_compiler.h>
extern struct Device * TimerBase;

#define ECLOCKTOF(ec)	\
	(((float) (ec)->ev_hi) + 0.000001f * ((float) (ec)->ev_lo))

static __inline__ void 
  EClockCalc(REG(d2, struct EClockVal * start),REG(a0, struct EClockVal * now ))
{
	float freq, sec;
	
	freq = ReadEClock( now );
	
	sec = (now->ev_hi - start->ev_hi - ((now->ev_lo < start->ev_lo)?1:0))
		* freq + (now->ev_lo - start->ev_lo) / freq;
	
	now->ev_hi	= (ULONG) sec;
	now->ev_lo	= (sec - now->ev_hi) * 1000000;
}

static struct EClockVal systb,start;

clock_t clock(void)
{
  struct EClockVal now;
  
  EClockCalc(&systb,&now);
  return ((ECLOCKTOF(&now)-ECLOCKTOF(&start)) * CLOCKS_PER_SEC);
}

void __initclock(void)
{
  ReadEClock(&systb);
  EClockCalc(&systb,&start);
}

ADD2INIT(__initclock,-3); // TimerBase init is -4

#else
#include <proto/exec.h>
#include <proto/timer.h>
extern struct Device * TimerBase;
static struct EClockVal start;

clock_t clock(void)
{
  struct EClockVal now;
  float f = ReadEClock(&now);
  return((now.ev_hi - start.ev_hi - !!(now.ev_lo < start.ev_lo))
    * f + ((now.ev_lo - start.ev_lo) / f)) * CLOCKS_PER_SEC;
}

void __initclock(void)
{
  ReadEClock(&start);
}

ADD2INIT(__initclock,-3); // TimerBase init is -4

#endif
