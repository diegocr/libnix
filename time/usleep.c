
#include <proto/alib.h>
#include <exec/errors.h>
#include <errno.h>
#include <signal.h>
#include "timerbase.h"

/**
 * This isn't a 100% implementation since alarm()/SIGALARM isnt supported
 * (yet!) on this current stdlib for amigift ...
 *
 * man:
 * http://www.opengroup.org/onlinepubs/007908799/xsh/usleep.html
 */

int usleep(unsigned long usec)
{
	int result;

	if( usec == 0 || usec >= 1000000 ) {

		errno = EINVAL;
		return -1;
	}

	result = TimeDelay(0,0, usec );

	if( result == IOERR_ABORTED )
		raise(SIGABRT);

	return result;
}
