#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

int vsprintf(char *s,const char *format,va_list args)
{ int retval;
  FILE buffer;
  buffer.p=buffer.buffer=s;
  buffer.incount=0;
  buffer.outcount=INT_MAX;
  buffer.flags=__SSTR|__SWR;
  buffer.linebufsize=0;
  buffer.magic=FILEMAGICID;
  retval=vfprintf(&buffer,format,args);
  fputc('\0',&buffer);
  return retval;
}
