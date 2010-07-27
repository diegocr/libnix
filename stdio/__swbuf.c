#include <stdio.h>
#include <errno.h>
#include <unistd.h> /* write */
#include <proto/dos.h>

int __swbuf(int c,FILE *stream) /* Get next output block */
{ int out,lbs;
/*  if(!__valid_fp(stream))
  { errno=EINVAL;
    return EOF;
  }*/

  if(stream->flags&(__SSTR|__SERR)) /* sprintf buffer | error on stream */
  { stream->outcount=0;
    errno=EPERM;
    return EOF;
  }else if(stream->flags&__SRD)
  {
    stream->incount=0; /* throw away input buffer */
    stream->tmpp=NULL;
    stream->flags&=~__SRD;
  }
  lbs=stream->flags&__SLBF?-stream->bufsize:0;
  out=(stream->flags&__SNBF?0:stream->bufsize-1)+lbs;
  if(!(stream->flags&__SWR)) /* File wasn't in write mode */
  { if(!stream->buffer)
    { unsigned char ch = (unsigned char)c;
      if(!(stream->flags&__SNBE))
      { char errstr[70];
	stream->flags|=__SNBE;
	Fault(ERROR_INVALID_LOCK,NULL,errstr,sizeof(errstr));
	fprintf(stream==stderr?stdout:stderr,"\afp(%p):%s\n",stream,errstr);
      }
      if(write(stream->file,&ch,1)<0)
      { stream->flags|=__SERR; /* error flag */
        return EOF; }
      return c;
    }
    stream->p=stream->buffer; /* set buffer */
    stream->outcount=--out;   /* and buffercount */
    stream->flags|=__SWR; }   /* and write mode */
  *stream->p++=c; /* put this character */
  if(stream->outcount<0&&(stream->outcount<lbs||(char)c=='\n'))
  { if(__fflush(stream)) /* Buffer full */
      return EOF;
    stream->p=stream->buffer; /* Set new buffer */
  }
  stream->linebufsize=lbs;
  stream->outcount=out;
  stream->flags|=__SWR;
  return c;
}
