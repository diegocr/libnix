#include "timerbase.h"
#include <sys/time.h>

extern int nanosleep( struct timespec *x, struct timespec *y );

unsigned int sleep(unsigned int seconds)
{
	struct timespec rqt, rmt;

	rqt.ts_sec  = seconds;
	rqt.ts_nsec = 0;

	nanosleep(&rqt, &rmt);

	return (unsigned int)rmt.ts_sec;
}
