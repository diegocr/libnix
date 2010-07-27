#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int fwrite(const void *ptr,size_t size,size_t nmemb,FILE *stream)
{
  signed long subsize;
  unsigned long total;
  unsigned char *b=(unsigned char *)ptr;
  if(!(total=size*nmemb)) /* Just in case size==0 */
    return total;
  if(!__valid_fp(stream)||!b)
    return 0;
  if((stream->flags&(__SWO|__SNBF))==(__SWO|__SNBF))
  { FILE fp;
    bzero(&fp,sizeof(fp));
    fp.magic=FILEMAGICID;
    fp.buffer=b;
    fp.p=b+total;
    fp.flags=__SWR|(stream->flags&~(__SWO|__SNBF));
    fp.file=stream->file;
    if(__fflush(&fp) || (fp.flags&__SERR))
    { stream->flags|=__SERR;
      return 0; }
    return nmemb;
  }
  do
  {
    if(stream->outcount>0)
    {
      subsize=total>stream->outcount?stream->outcount:total;
      memcpy(stream->p,b,subsize);
      stream->p+=subsize;
      stream->outcount-=subsize;
      b+=subsize;
      total-=subsize;
    }else
    {
      int c;
      c=*b++;
      if(putc(c,stream)==EOF)
        break;
      total--;
    }
  }while(total);
  return (b-(unsigned char *)ptr)/size;
}
