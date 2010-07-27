
#include "socketnix.h"
#include "stabs.h"

long ioctlsocket(long d, u_long request, char *argp)
{
  return IoctlSocket(d,request,argp);
}
ALIAS(IoctlSocket,ioctlsocket);
