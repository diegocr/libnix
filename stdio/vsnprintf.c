
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

int vsnprintf(char *s,size_t size,const char *format,va_list args)
{
  int retval;
  FILE buffer;
  if((signed)size < 1)
  { errno = EINVAL;
    return -1; }
  buffer.p=buffer.buffer=s;
  buffer.incount=0;
  buffer.outcount=size-1;
  buffer.flags=__SSTR|__SWR;
  buffer.linebufsize=0;
  buffer.magic=FILEMAGICID;
  retval=vfprintf(&buffer,format,args);
  buffer.outcount++;
  fputc('\0',&buffer);
  if(retval+1 == size)
    retval++;
  return retval;
}
