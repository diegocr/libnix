
/**
 * nanosleep implementation
 * (C)2006 Diego Casorran <diegocr()users.sf.net>
 * Brought to you under the BSD Licence.
 */

#include <devices/timer.h>
#include <clib/alib_protos.h>
#include <proto/dos.h>
#include <exec/errors.h>

#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include "timerbase.h"

#define NSEC_PER_USEC   1000            /* nanoseconds per microsecond */
#define USEC_PER_SEC    1000000         /* microseconds per second */
#define NSEC_PER_SEC    1000000000      /* nanoseconds per second */

int nanosleep(struct timespec *requested_time, struct timespec *remaining_time)
{
	struct timeval reqtv;
	struct timeval f,s;
	long rs = 0, ru = 0 ;
	int rc;

	if((requested_time == NULL)
	|| (requested_time->ts_sec < 0)
	|| (requested_time->ts_nsec > NSEC_PER_SEC)) {
		errno = EINVAL;
		return -1;
	}

	if(remaining_time != NULL) {
		GetSysTime(&f);
	}

	reqtv.tv_sec	= requested_time->ts_sec;
	reqtv.tv_usec	= requested_time->ts_nsec / 1000;

	rc = TimeDelay(0, reqtv.tv_sec, reqtv.tv_usec );

	if( rc < 0 ) {

		if(remaining_time != NULL) {

			GetSysTime(&s);

			rs	= reqtv.tv_sec	- (s.tv_sec-f.tv_sec);
			ru	= reqtv.tv_usec	- (s.tv_usec-f.tv_usec);

			rs	+= ru / 1000000L;
			ru	%= 1000000L;
		}

		if( rc == IOERR_ABORTED )
			errno = EINTR;
		else	errno = EBUSY; /* non nanosleep standard */

		rc = -1;
	}

	if(remaining_time != NULL) {

		remaining_time->ts_sec	= rs;
		remaining_time->ts_nsec	= (ru * 1000);
	}

	return( rc );
}
