
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#if __GNUC__ < 3
# define __RELID(S) \
	static const char __Id[] __attribute__((unused)) = S
#else
# define __RELID(S) \
	static char __Id[] __attribute__((used)) = S
#endif
__RELID("$Id: psem,v 0.3 2009/01/08 00:01:02 diegocr Exp $");

#include <proto/exec.h>	/* imp core */
#include <proto/alib.h>	/* NewList */
#include <proto/dos.h>	/* FilePart */
#include <string.h>	/* memset strcmp */
#include <stdio.h>	/* snprintf */
#include <stdlib.h>	/* atexit */
#include "psem.h"

const unsigned long _PSEM_MAGIC = __PSEM_MAGIC;
static unsigned __psem_uniqueid = 0;
struct MinList *__psem_list = NULL;

// #undef FreeVec
// #define FreeVec(s) DBG("---FreeVec(%s);\n",#s)

static void __psem_list_destruct( void )
{
	psem_node_t *pn;
	struct Task *ThisTask = FindTask(NULL);
	
	/**
	 * The program using me is about to exit, proceed 
	 * to safely clean up everything made by myself..
	 */
	
	ENTER();
	
	Forbid();
	while((pn = (psem_node_t *)RemTail((struct List *)__psem_list)))
	{
		/**
		 * Check if this semaphore was created by this task, 
		 * and proceed to destroy it if so. but before we'll
		 * try to release any unlocked semaphore by the user..
		 */
		
		POINTER(pn);
		while(_SSEM(pn->sem)->ss_Owner == ThisTask)
		{
			DBG("Releasing sema...\n");
			ReleaseSemaphore(_SSEM(pn->sem));
		}
		
		if(pn->sem->owner == (unsigned)ThisTask)
		{
			VALUE(_SSEM(pn->sem)->ss_QueueCount);
			if(_SSEM(pn->sem)->ss_QueueCount != -1)
			{
				/**
				 * The Semaphore is locked by someone!
				 * we'll not exit unless it's unlocked!
				 */
				
				struct timespec ts;
				struct SemaphoreRequest *sr;
				unsigned waiters = 0, owned = 0;
				
				sr = (struct SemaphoreRequest *)_SSEM(pn->sem)->ss_WaitQueue.mlh_Head;
				while(sr->sr_Link.mln_Succ)
				{
					waiters++;
					
					/**
					 * hmm, is this a Shared Semaphore and the task
					 * didn't released all obtained semaphores?..
					 */
					
					if(sr->sr_Waiter == ThisTask)
					{
						owned++;
					}
					else if( sr->sr_Waiter != NULL )
					{
						Signal(sr->sr_Waiter,SIGBREAKF_CTRL_C);
					}
					
					sr = (struct SemaphoreRequest *)sr->sr_Link.mln_Succ;
				}
				
				if( waiters != owned )
				{
					VALUE(waiters);
					
					Permit();
					do {
						ts.ts_sec = time(NULL) + 120;
						ts.ts_nsec = 1000;
						
					} while(sem_timedwait((sem_t *)pn->sem,&ts));
					Forbid();
				}
			}
			
			psem_destroy(pn->sem);
		}
		
		FreeVec(pn->sem);
		FreeVec(pn);
	}
	Permit();
	
 	FreeVec(__psem_list);
	
	LEAVE();
}

void psem_list_add(psem_t *sem)
{
	psem_node_t *pn;
	
	/* This is executed in forbid state! */
	
	ENTER();
	POINTER(sem);
	VALUE(sem->semid);
	
	if(__psem_list == NULL)
	{
		if(!(__psem_list = AllocVec(sizeof(*__psem_list),MEMF_PUBLIC)))
			return;
		
		NewList((struct List *)__psem_list);
		atexit(__psem_list_destruct);
		POINTER(__psem_list);
	}
	
	if((pn = AllocVec(sizeof(*pn),MEMF_PUBLIC)))
	{
		pn->sem = sem;
		AddTail((struct List *)__psem_list,(struct Node *)pn);
	}
	
	LEAVE();
}

static void psem_list_rem(psem_t *sem)
{
	psem_node_t *pn,*pnn;
	
	/* This is executed in forbid state! */
	
	ENTER();
	
	if(__psem_list == NULL)
		return;
	
	POINTER(sem);
	
	pn = (psem_node_t *)__psem_list->mlh_Head;
	while((pnn = (psem_node_t *)pn->node.mln_Succ))
	{
		if( pn->sem->semid == sem->semid )
		{
			Remove((struct Node *)pn);
			VALUE(pn->sem->semid);
 			memset(pn->sem,_PSEM_ERASEDBIT,sizeof(psem_t));
// 			FreeVec(pn->sem);
			FreeVec(pn);
			break;
		}
		
		pn = pnn;
	}
	
	LEAVE();
}

char *psem_name(const char *name,char *out,unsigned len)
{
	if( name == NULL )
	{
		struct Task *ThisTask;
		struct Process *ThisProcess;
		
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
						name = (const char *)taskname;
					}
				}
			}
			
			if(!(name && *name))
				name = (const char *)ThisTask->tc_Node.ln_Name;
		}
		
		if(!name || *name < 32)
			name = (const char *)"unnamed";
	}
	
	snprintf( out, len, _PSEM_NAME, FilePart(name));
	
	return(out);
}

psem_t *psem_init(const char *name, int shared, unsigned int value)
{
	psem_t *s;
	
	ENTER();
	
	if((s = AllocVec(sizeof(psem_t),MEMF_PUBLIC|MEMF_CLEAR)))
	{
		if( name == NULL )
		{
			s->flags |= SEMF_UNNAMED;
		}
		if( shared )
		{
			s->flags |= SEMF_SHARED;
		}
		
		Forbid();
		_SSEM(s)->ss_Link.ln_Name = psem_name(name,s->name,sizeof(s->name)-1);
		_SSEM(s)->ss_Link.ln_Pri = 1;
		AddSemaphore(_SSEM(s));
		
		s->value = value;
		s->magic = _PSEM_MAGIC;
		s->owner = (unsigned)FindTask(NULL);
		if(__psem_uniqueid < 1)
			__psem_uniqueid = s->owner;
		s->semid = ++__psem_uniqueid;
		
		psem_list_add(s);
		Permit();
	}
	
	if(s == NULL)
		s = (psem_t *)SEM_FAILED;
	
	RETURN(s);
	return((void *)s);
}

void psem_destroy(psem_t *sem)
{
	ENTER();
	POINTER(sem);
	
	if((sem->owner == (unsigned)FindTask(NULL)) || (sem->flags & SEMF_OPENDUP))
	{
		VALUE(sem->flags);
		
		if(!(sem->flags & SEMF_OPENDUP))
		{
			DBG("Trying to own the semaphore to be removed...\n");
			ObtainSemaphore(_SSEM(sem));
		}
		else
		{
			DBG("Destroying sem_open()'ed semaphore...\n");
		}
		
		Forbid();
		if(!(sem->flags & SEMF_OPENDUP))
		{
			RemSemaphore(_SSEM(sem));
		}
		psem_list_rem(sem);
		memset(sem,_PSEM_ERASEDBIT,sizeof(*sem));
		Permit();
	}
	
	LEAVE();
}
