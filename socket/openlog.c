
#include <stdlib.h>
#include <errno.h>
#include <proto/socket.h>
#include <amitcp/socketbasetags.h>

void openlog(const char *ident, int logstat, int logfac)
{
	long r;
	
	r = SocketBaseTags(SBTM_SETVAL(SBTC_LOGTAGPTR),		(ULONG)ident,
				SBTM_SETVAL(SBTC_LOGSTAT),	logstat,
				SBTM_SETVAL(SBTC_LOGFACILITY),	logfac,
				TAG_DONE);
	
	errno = ( r ? EINVAL : 0 );
}

