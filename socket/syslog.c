
#include <stdlib.h>
#include <stdarg.h>
#include <proto/socket.h>

void syslog(ULONG pri, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsyslog(pri, fmt, ap);
	va_end(ap);
}

