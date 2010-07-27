
#include <stdlib.h>
#include <proto/socket.h>
#include <amitcp/socketbasetags.h>

void closelog(void)
{
	SocketBaseTags(SBTM_SETVAL(SBTC_LOGTAGPTR), NULL, TAG_DONE);
}
