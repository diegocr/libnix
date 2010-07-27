#include <stdio.h>

int fputs(const char *s,FILE *stream)
{
  if(!s)
    return 0;
  if((stream->flags&(__SWO|__SNBF))==(__SWO|__SNBF))
  { FILE fp;
    bzero(&fp,sizeof(fp));
    fp.magic=FILEMAGICID;
    fp.buffer=(char *)s;
    fp.p=(char *)s+strlen(s);
    fp.flags=__SWR|(stream->flags&~(__SWO|__SNBF));
    fp.file=stream->file;
    if(__fflush(&fp) || (fp.flags&__SERR))
    { stream->flags|=__SERR;
      return EOF; }
    return 0;
  }
  while(*s)
  { int c = *s++;
    if(putc(c,stream)==EOF)
      return EOF; }
  return 0;
}
