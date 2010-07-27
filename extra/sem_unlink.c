
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include "psem.h"
#include <string.h>	/* strlen */

int sem_unlink (const char *__name)
{
	unsigned char name[_PSEM_NAME_MAXLENGTH];
	struct SignalSemaphore *ssem;
	psem_t *psem = NULL;
	
	if(strlen(__name) > (_PSEM_NAME_MAXLENGTH-20))
	{
		errno = ENAMETOOLONG;
		return -1;
	}
	
	psem_name(__name,name,sizeof(name)-1);
	
	Forbid();
	if((ssem = FindSemaphore((STRPTR)name)))
	{
		//if(((psem_t *)ssem)->magic == _PSEM_MAGIC)
			psem = (psem_t *)ssem;
	}
	Permit();
	
	if(_PSEM_INVALID(psem))
	{
		errno = ((ssem != NULL) ? EACCES:ENOENT);
		return -1;
	}
	
	Forbid();
	if(psem_trywait(psem)==0)
	{
		psem_destroy(psem);
	}
	else
	{
		/**
		 * Could a semaphore be unlinked by a process other
		 * than the one who created it? if so, this lib will
		 * require some rework...
		 */
		
		if(psem->owner == (unsigned)FindTask(NULL))
			psem->flags |= SEMF_EXPUNGE;
	}
	Permit();
	
	return 0;
}
