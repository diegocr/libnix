
#include "socketnix.h"
#include "stabs.h"

long closesocket(long d)
{
  shutdown(d,2);
  return CloseSocket(d);
}
ALIAS(CloseSocket,closesocket);
