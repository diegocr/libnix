#include <proto/exec.h>
#include <proto/alib.h>
#include <proto/timer.h>
#include <stdlib.h>
#include <stdio.h> /* for stderr */
#include "stabs.h"

struct Device *TimerBase = NULL;

struct MsgPort *__timermp = NULL;
struct timerequest *__timereq = NULL;

static BOOL oderr = TRUE;
extern ULONG __timerunit;

CONSTRUCTOR(initTimer)
{
  //if((mp = (struct MsgPort *)CreatePort( NULL, 0 )))
  if((__timermp = AllocVec(sizeof(*__timermp),MEMF_ANY|MEMF_PUBLIC|MEMF_CLEAR)))
  {
    __timermp->mp_Node.ln_Type	= NT_MSGPORT;
    __timermp->mp_Flags		= PA_SIGNAL;
    __timermp->mp_SigBit	= SIGB_SINGLE;
    __timermp->mp_SigTask	= FindTask(NULL);
    NewList(&__timermp->mp_MsgList);
    if((__timereq = (struct timerequest *)CreateExtIO(__timermp, sizeof(struct timerequest))))
    {
      oderr = OpenDevice( TIMERNAME,__timerunit,(struct IORequest *)__timereq, 0);
      if(!oderr)
      {
	TimerBase = __timereq->tr_node.io_Device;
      }
    }
  }

  if( TimerBase == NULL )
  {
    fprintf( stderr, TIMERNAME );
    exit(20);
  }
}

DESTRUCTOR(closeTimer)
{
  if(__timereq)
  {
    if(!oderr)
      CloseDevice( (struct IORequest *)__timereq);
    DeleteExtIO( (struct IORequest *)__timereq);
    __timereq = NULL;
  }

  if(__timermp)
  {
    FreeVec(__timermp);
    __timermp = NULL;
  }

  TimerBase = NULL;
  oderr = TRUE;
}
