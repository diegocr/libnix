
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */
//
/**
 * REFERENCES:
 * 
 * http://www.kernel.org/doc/man-pages/online/pages/man7/sem_overview.7.html
 * http://www.linuxdevcenter.com/pub/a/linux/2007/05/24/semaphores-in-linux.html
 * http://www.cs.cf.ac.uk/Dave/C/node26.html (System V, mainly)
 */

#ifndef AMIGASF_SEMAPHORE_H
#define AMIGASF_SEMAPHORE_H 1

#include <proto/exec.h>
#include <exec/semaphores.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>	/* abort */

#define _PSEM_NAME 			"POSIX Semaphore [%s]"
#define _PSEM_NAME_MAXLENGTH		63
#define _PSEM_ERASEDBIT			0x7f
#define _SSEM(PSEM)			((struct SignalSemaphore *)(PSEM))

#define _PSEM_INVALID(PSEM)	\
	(((PSEM) == NULL) ? TRUE : ((((psem_t *)PSEM)->magic == _PSEM_MAGIC) ? FALSE \
		: ((((psem_t *)PSEM)->name[0] == _PSEM_ERASEDBIT) ? abort(),TRUE:TRUE)))

#ifdef DEBUG
# include <stdio.h>
# define DBG(a...) fprintf(stderr,a)
#else /* DEBUG */
# define DBG(a...) ((void)0)
#endif /* DEBUG */

#define FILELINEFUNC	__FILE__,__LINE__,__PRETTY_FUNCTION__
#define ENTER()		DBG("%s,%d(%s): Entering...\n",FILELINEFUNC)
#define LEAVE()		DBG("%s,%d(%s): Leaving...\n",FILELINEFUNC)
#define RETURN(x)	DBG("%s,%d(%s): Leaving, Rc=%ld,%lx\n",FILELINEFUNC,(long)x,(long)x)
#define VALUE(x)	DBG("%s,%d(%s): '%s' = 0x%08lx,%ld\n",FILELINEFUNC,#x,(long)x,(long)x)
#define POINTER(x)	DBG("%s,%d(%s): '%s' = 0x%08lx\n",FILELINEFUNC,#x,(long)x)

typedef struct {
	
	struct SignalSemaphore ssem;
	
	unsigned long magic;
	#define __PSEM_MAGIC	0x53454D00
	
	unsigned char name[_PSEM_NAME_MAXLENGTH+1];
	
	unsigned int flags;
	unsigned int value;
	unsigned int semid;
	unsigned int owner;
	
} psem_t;

typedef struct {
	struct MinNode node;
	psem_t *sem;
} psem_node_t;

enum {
	SEMF_SHARED  = (1L << 15), // semaphore is shared type
	SEMF_UNNAMED = (1L << 16), // sem_init()'s semaphore
	SEMF_EXPUNGE = (1L << 23), // sem_unlink()'ed semaphore
	SEMF_OPENDUP = (1L << 24), // sem_open() not owned
};

extern const unsigned long _PSEM_MAGIC;
extern struct MinList *__psem_list;

__BEGIN_DECLS
extern void psem_list_add(psem_t *sem);
extern psem_t *psem_init(const char *name, int shared, unsigned int value);
extern void psem_destroy(psem_t *sem);
extern char *psem_name(const char *name,char *out,unsigned len);

extern __inline int psem_trywait(psem_t *sem)
{
	ENTER();
	POINTER(sem);
	
	if(sem->flags & SEMF_SHARED)
	{
		if(AttemptSemaphoreShared(_SSEM(sem)))
			return 0;
	}
	else
	{
		if(AttemptSemaphore(_SSEM(sem)))
			return 0;
	}
	
	LEAVE();
	errno = EAGAIN;
	return -1;
}

extern __inline int psem_wait(psem_t *sem)
{
	ENTER();
	POINTER(sem);
	
	if(sem->flags & SEMF_SHARED)
	{
		ObtainSemaphoreShared(_SSEM(sem));
	}
	else
	{
		ObtainSemaphore(_SSEM(sem));
	}
	
	LEAVE();
	return 0;
}

// get the correct psem pointer...
extern __inline psem_t *sem2psem(sem_t *sem)
{
	psem_t *psem = NULL;
	
	if(!_PSEM_INVALID(sem) && (__psem_list != NULL))
	{
		psem_node_t *pn;
		
		pn = (psem_node_t *)__psem_list->mlh_Head;
		while(pn->node.mln_Succ)
		{
			if( pn->sem->semid == ((psem_t *)sem)->semid )
			{
				psem = pn->sem;
				break;
			}
			
			pn = (psem_node_t *)pn->node.mln_Succ;
		}
	}
	
	return(psem);
}

__END_DECLS

#endif /* AMIGASF_SEMAPHORE_H */
