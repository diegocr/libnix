#include <proto/socket.h>
#include <netdb.h>

#ifndef gethostbyname
# error what !?
#endif

static __inline struct hostent *GetHostByName(const char *name)
{
  return gethostbyname(name);
}

#undef gethostbyname
struct hostent *gethostbyname(const char *name)
{
  return GetHostByName(name);
}

