/* 10-Apr-94 bug fix M. Fleischer
 * 11-Apr-94 bug fix & readjustment G. Nikl
 * 14-Apr-94 readjustment M. Fleischer
 * 24-Apr-94 cleanup for malloc changed
 */

#include <stdlib.h>
#include <stdio.h>
#include <exec/lists.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/alib.h>
#include "stabs.h"
#include "debuglib.h"

extern ULONG _MSTEP;

static struct MinList __memorylist;

#define USE_SEMAPHORE 0
#if USE_SEMAPHORE
static struct SignalSemaphore  *__memsema;
#endif

void *malloc(size_t size)
{
  register struct MinNode *node;
  struct MemHeader *b;
  ULONG size2/*,f*/,*a = NULL;

  if((signed)size < 1) {
    PrintFault(ERROR_OBJECT_TOO_LARGE,"malloc");
    abort();
  }

#if USE_SEMAPHORE
  ObtainSemaphore(__memsema);
#endif
  node=__memorylist.mlh_Head;
  size+=sizeof(ULONG);
  while(node->mln_Succ) /* yet some memory in my list ? */
  {
    if((a=Allocate((struct MemHeader *)node,size))!=NULL)
    {
      *a++=size;
#if USE_SEMAPHORE
	  goto end;
#else
	return a;
#endif
    }
    node=node->mln_Succ;
  }
  size2=sizeof(struct MemHeader)+sizeof(ULONG)+size; /* Total memory needed */
  if(size2<_MSTEP)
  { size2=_MSTEP; /* Allocate a _MSTEP bytes large block if possible */
    //f=MEMF_PUBLIC|MEMF_CLEAR;
  } //else { f=MEMF_PUBLIC;}
  size2=(size2+4095)&~4095; /* Blow up to full MMU Page */
  if((b=(struct MemHeader *)AllocMem(size2,MEMF_ANY))!=NULL)
  {
    b->mh_Lower=b->mh_First=(struct MemChunk *)(b+1);
    b->mh_First->mc_Next=NULL;
    b->mh_Free=b->mh_First->mc_Bytes=size2-sizeof(struct MemHeader);
    b->mh_Upper=(char *)b+size2;
    AddHead((struct List *)&__memorylist,&b->mh_Node);
    a=Allocate(b,size); /* It has to work this time */
	if (a != NULL) {
		*a++=size;
	}
  }

#if USE_SEMAPHORE
 end:
  ReleaseSemaphore(__memsema);
#endif
  return a;
}


void free(void *ptr)
{ struct MemHeader *a;

  if(!ptr) /* What does that mean ????? */
  { DB( BUG("NULL pointer free'd\n"); )
    return; }

#if USE_SEMAPHORE
  ObtainSemaphore(__memsema);
#endif
  a=(struct MemHeader *)__memorylist.mlh_Head;
  for(;;)
  {
    if(((struct MinNode *)a)->mln_Succ==NULL) /* Is not in list ????? */
    { DB( BUG("Fake memory free'd\n"); )
      return; }

    if(ptr>=a->mh_Lower&&ptr<a->mh_Upper) /* Entry found */
      break;

    a=(struct MemHeader *)((struct MinNode *)a)->mln_Succ;
  }

  DB( memset(ptr,0xcc,((ULONG *)ptr)[-1]); ) /* Destroy contents */

  Deallocate(a,(ULONG *)ptr-1,((ULONG *)ptr)[-1]);
  if(a->mh_Free==(char *)a->mh_Upper-(char *)a->mh_Lower) /* All free ? */
  { Remove(&a->mh_Node);
    FreeMem(a,(char *)a->mh_Upper-(char *)a);
  }
#if USE_SEMAPHORE
  ReleaseSemaphore(__memsema);
#endif
}


extern char __dosname[];
extern char *__procname;

void __initmalloc(void)
{
	struct Library *DOSBase = OpenLibrary(__dosname, 0);
	NewList((struct List *)&__memorylist);
#if USE_SEMAPHORE
	__memsema = AllocMem(sizeof(struct SignalSemaphore), MEMF_PUBLIC | MEMF_CLEAR);
	if(!__memsema)exit(1234);
	InitSemaphore(__memsema);
#endif
	if(__procname[0]=='B'&&__procname[10]==' ')
	{
		struct Task *ThisTask;
		struct Process *ThisProcess;
		char *name=NULL;

		ThisProcess = (struct Process *)(ThisTask = FindTask(NULL));

		if((ThisTask->tc_Node.ln_Succ &&
			(ThisTask->tc_Node.ln_Type==NT_PROCESS||
			 ThisTask->tc_Node.ln_Type==NT_TASK)))
		{
			if(ThisTask->tc_Node.ln_Type==NT_PROCESS && ThisProcess->pr_CLI)
			{
				struct CommandLineInterface *cli =
					(struct CommandLineInterface *)BADDR(ThisProcess->pr_CLI);

				if( cli && cli->cli_Module && cli->cli_CommandName )
				{
					char *taskname = (char *)(cli->cli_CommandName<<2);
					int tnl = *taskname++;

					if( tnl > 0 )
					{
						name = taskname;
					}
				}
			}

			if(!(name && *name))
				name = ThisTask->tc_Node.ln_Name;
		}

		if(name && *name > 31)
			name = FilePart(name);
		if(name&&*name)
			__procname=name;
	}
	CloseLibrary(DOSBase);
	DOSBase = NULL;
}

void __exitmalloc(void)
{ struct MemHeader *a;
  while((a=(struct MemHeader *)RemHead((struct List *)&__memorylist))!=NULL)
    FreeMem(a,(char *)a->mh_Upper-(char *)a); /* free all memory */
#if USE_SEMAPHORE
  FreeMem(__memsema, sizeof(struct SignalSemaphore));
#endif
}

ADD2EXIT(__exitmalloc,-50);
ADD2INIT(__initmalloc,-50);
