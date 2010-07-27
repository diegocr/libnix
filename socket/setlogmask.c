
#include <stdlib.h>
#include <sys/socket.h>
#include <proto/socket.h>
#include <amitcp/socketbasetags.h>

int
setlogmask(int pmask){
  ULONG taglist[5];

  taglist[0] = SBTM_GETVAL(SBTC_LOGMASK);
  taglist[2] = SBTM_SETVAL(SBTC_LOGMASK);
  taglist[3] = pmask;
  taglist[4] = TAG_END;

  SocketBaseTagList((struct TagItem *)taglist);
  return (int)taglist[1];
}
