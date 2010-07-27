
#include "socketnix.h"

int inet_pton(int af, const char *src, void *dst)
{
    if (af != AF_INET) {
	errno = EAFNOSUPPORT;
	return -1;
    }
    return inet_aton (src, dst);
}
