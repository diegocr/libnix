#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> /* If you use normal file I/O,
                       the memory functions don't count that much on memory */
#include <exec/lists.h>
#include <proto/exec.h>
#include "debuglib.h"
#include "stabs.h"

struct MinList __filelist = { /* list of open files (fflush() needs also access) */
  (struct MinNode *)&__filelist.mlh_Tail,
  NULL,
  (struct MinNode *)&__filelist.mlh_Head
};

FILE *fopen(const char *filename,const char *mode)
{ struct filenode *node = (struct filenode *)calloc(1,sizeof(*node));
  if(node!=NULL)
  { bzero( node, sizeof(*node));
    if((node->FILE.buffer=(char *)malloc(BUFSIZ))!=NULL)
    { node->FILE.bufsize=BUFSIZ;
      node->FILE.flags|=__SMBF; /* Buffer is malloc'ed */
      node->FILE.magic=FILEMAGICID;
      if(freopen(filename,mode,&node->FILE)!=NULL)
      { AddHead((struct List *)&__filelist,(struct Node *)&node->node);
        return &node->FILE; }
      free(node->FILE.buffer);
    }
    else
      errno=ENOMEM;
    free(node);
  }
  else
    errno=ENOMEM;
  return NULL;
}

#define SS(s) (((s)->file<=STDERR_FILENO)&&!eXit)
static int eXit = 0;

int fclose(FILE *stream)
{ struct filenode *node;
  int retval;
  if(!stream)
  { DB( BUG("NULL pointer fclose'd\n"); )
    return EOF; }
  if(stream->magic!=FILEMAGICID||SS(stream))
  { fprintf(stderr,"fp(%p): illegal fclose\n",stream);
    errno=EINVAL;
    if(eXit)abort();
    return EOF;
  }
  retval=freopen(NULL,NULL,stream)==NULL?EOF:0;
  if(stream->flags&__SMBF) /* Free buffer if necessary */
  { free(stream->buffer);
    stream->buffer=NULL; }
  node=(struct filenode *)((struct MinNode *)stream-1);
  Remove((struct Node *)&node->node);
  memset( node, 0x7f, sizeof(*node));
  stream->magic=0xdead;
  free(node);
  return retval;
}

void __exitstdfio(void)
{ struct MinNode *node; eXit=1;
  while((node=__filelist.mlh_Head)->mln_Succ!=NULL)
    fclose(&((struct filenode *)node)->FILE);
}

/* Call our private destructor at cleanup */
ADD2EXIT(__exitstdfio,-20);
