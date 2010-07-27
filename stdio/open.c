#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#define DEVICES_TIMER_H
#include <dos/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <stdio.h>
#include "stabs.h"
#include <signal.h>
#include <dos/dos.h>

/*
**
*/
extern void __seterrno(void);

/*
**
*/
static StdFileDes **stdfiledes;
static unsigned long stdfilesize=0;
static long stderrdes=0; /* The normal Amiga shell sets no process->pr_CES stream -
                          * we use Open("*",MODE_NEWFILE) in this case
                          */

/*
**
*/
static void _setup_file(StdFileDes *fp)
{ fp->lx_inuse  = 1;
  fp->lx_isatty = IsInteractive(fp->lx_fh) ? -1 : 0;
}

/*
**
*/
static __inline StdFileDes *_allocfd(void)
{ StdFileDes *fp,**sfd;
  int file,max;

  for(sfd=stdfiledes,max=stdfilesize,file=3;file<max;file++)
    if(!sfd[file] || !sfd[file]->lx_inuse)
      break;

  if(file>SHRT_MAX)
  { errno=EMFILE;
    return NULL;
  }

#define ADDFD 10
  if(file==max)
  { if((sfd=realloc(stdfiledes,(file+ADDFD)*sizeof(fp)))==NULL)
    { errno=ENOMEM;
      return NULL;
    }
    stdfiledes=sfd;
    stdfilesize+=ADDFD;
    for(max=file;max<stdfilesize;max++)
      sfd[max] = NULL;
  }

  if((fp=sfd[file])==NULL)
  { if((sfd[file]=fp=malloc(sizeof(*fp)))==NULL)
    { errno=ENOMEM;
      return NULL;
    }
    fp->lx_pos = file;
  }

  return fp;
}

/*
**
*/

#define IXFA(f) \
  ((f&O_TRUNC)?MODE_NEWFILE:((f&O_CREAT)?MODE_READWRITE:MODE_OLDFILE))

int open(const char *path,int flags,...)
{ StdFileDes *sfd;

#ifdef IXPATHS
  extern char *__amigapath(const char *path);
  if((path=__amigapath(path))==NULL)
    return -1;
#endif

  if(flags & O_EXCL)
  { BPTR lock;
    if((lock=Lock(path,SHARED_LOCK)))
    { UnLock(lock);
      errno=EEXIST;
      return EOF;
    }
  }

  if((sfd=_allocfd()))
  { sfd->lx_sys=0;
    sfd->lx_oflags=flags;
    if((sfd->lx_fh=Open((char *)path,IXFA(flags))))
    {
      if(!(flags & O_APPEND)||Seek(sfd->lx_fh,0,OFFSET_END)!=-1)
      { _setup_file(sfd);
        return sfd->lx_pos;
      }
      Close(sfd->lx_fh);
    }
    __seterrno();
    sfd->lx_inuse = 0;
  }

  return -1;
}

int close(int d)
{ StdFileDes *sfd = d>STDERR_FILENO?_lx_fhfromfd(d):NULL;

  if (sfd) {
    if (!(sfd->lx_inuse-=1)) {
      if (sfd->lx_pos=d,!sfd->lx_sys) {
        if (!Close(sfd->lx_fh)) {
          __seterrno(); return EOF;
        }
      }
    }
    else {
      stdfiledes[d] = 0;
    }
  }

  return 0;
}

ssize_t read(int d,void *buf,size_t nbytes)
{ StdFileDes *sfd = _lx_fhfromfd(d);

  if (sfd) {
    long r;
    __chkabort();
    if((r=Read(sfd->lx_fh,buf,nbytes))!=EOF)
      return r;
    __seterrno();
  }

  return EOF;
}

ssize_t write(int d,const void *buf,size_t nbytes)
{ StdFileDes *sfd = _lx_fhfromfd(d);

  if (sfd) {
    long r;
#if 0 // libnix < 3.0x
    __chkabort();
    switch((sfd->lx_oflags&O_APPEND)!=0) {
      case 1:
        if(!sfd->lx_isatty&&(Seek(sfd->lx_fh,0,OFFSET_END)==EOF))
          break;
      default:
        if((r=Write(sfd->lx_fh,(char *)buf,nbytes))!=EOF)
          return r;
    }
#else
    if(SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
      raise(SIGINT);
    if((r = Write(sfd->lx_fh,(char *)buf,nbytes)) != EOF)
      return r;
#endif
    __seterrno();
  }

  return EOF;
}

off_t lseek(int d,off_t offset,int whence)
{ StdFileDes *sfd = _lx_fhfromfd(d);

  if (sfd) {
    long r,file=sfd->lx_fh;
//    __chkabort();
    if (Seek(file,offset,whence==SEEK_SET?OFFSET_BEGINNING:
                         whence==SEEK_END?OFFSET_END:OFFSET_CURRENT)!=EOF)
      if ((r=Seek(file,0,OFFSET_CURRENT))!=EOF)
        return r;
    __seterrno();
  }

  return EOF;
}

int ftruncate(int fd, off_t length)
{
  StdFileDes *sfd = _lx_fhfromfd(fd);
  long pos,r,file;

  if(!sfd)
  { errno = EBADF;
    return EOF;
  }
  file=sfd->lx_fh;
  pos=Seek(file,0,OFFSET_CURRENT);

  if((r=SetFileSize(file,length,OFFSET_BEGINNING))==EOF)
  { errno=EIO;
    return EOF;
  }
  if((r=Seek(file,length>pos?pos:length,OFFSET_BEGINNING))==EOF)
  { errno=EIO;
    return EOF;
  }
  return 0;
}

int truncate(const char *path, off_t length)
{
  int fd,r;

  if(!(path && *path))
  { errno=EFAULT;
    return EOF;
  }

  if((fd=open(path,0))==EOF)
  { errno=ENOENT;
    return EOF;
  }

  r=ftruncate(fd,length);
  close(fd);

  return r;
}

int isatty(int d)
{ StdFileDes *sfd = _lx_fhfromfd(d);

  return sfd?sfd->lx_isatty:0;
}

/*
**
*/
int _lx_addflags(int d,int oflags)
{ StdFileDes *sfd = _lx_fhfromfd(d);

  return sfd?sfd->lx_oflags|=oflags:0;
}

/*
** convert fd to a StdFileDes
*/
StdFileDes *_lx_fhfromfd(int d)
{ if(d<(int)stdfilesize)
  { StdFileDes *sfd=stdfiledes[d];
    if(sfd&&sfd->lx_inuse)
      return sfd; }
  return NULL;
}

/*
**
*/
void __initstdio(void)
{ extern struct WBStartup *_WBenchMsg;
  StdFileDes *fp,**sfd;

  if((stdfiledes=sfd=(StdFileDes **)malloc(3*sizeof(StdFileDes *)))) {
    if((sfd[STDIN_FILENO]=fp=(StdFileDes *)malloc(sizeof(StdFileDes)))) {
      fp->lx_fh     = Input();
      fp->lx_pos    = STDIN_FILENO;
      fp->lx_sys    = -1;
      fp->lx_oflags = O_RDONLY;
      _setup_file(fp);
      if((sfd[STDOUT_FILENO]=fp=(StdFileDes *)malloc(sizeof(StdFileDes)))) {
        fp->lx_fh     = Output();
        fp->lx_pos    = STDOUT_FILENO;
        fp->lx_sys    = -1;
        fp->lx_oflags = O_WRONLY;
        _setup_file(fp);
        if((sfd[STDERR_FILENO]=fp=(StdFileDes *)malloc(sizeof(StdFileDes)))) {
          if((fp->lx_fh=((struct Process *)FindTask(NULL))->pr_CES)==0)
            if(_WBenchMsg||(fp->lx_fh=stderrdes=Open("CONSOLE:",MODE_OLDFILE))==0)
              fp->lx_fh=sfd[STDOUT_FILENO]->lx_fh;
          fp->lx_pos    = STDERR_FILENO;
          fp->lx_sys    = -1;
          fp->lx_oflags = O_WRONLY;
          _setup_file(fp);
          stdfilesize += 3; return;
        }
      }
    }
  }
  exit(20);
}
ADD2INIT(__initstdio,-30);

void __exitstdio(void)
{ int i,max;

  for(max=stdfilesize,i=3;i<max;i++) {
    StdFileDes *sfd = stdfiledes[i];
    if(sfd && sfd->lx_inuse) {
      close(i);
    }
  }

  if(stderrdes)
    Close(stderrdes);
}
ADD2EXIT(__exitstdio,-30);

#if 0
#if __GNUC__ < 3
# define UNUSED __attribute__((unused))
#else
# define UNUSED __attribute__((used))
#endif
static unsigned char __auth[] UNUSED = "$(diegocr:libnix)";
#endif
