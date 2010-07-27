
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include "psem.h"

int sem_getvalue (sem_t *__sem, int *__sval)
{
	psem_t *sem = sem2psem(__sem);
	
	if(_PSEM_INVALID(sem) || (__sval == NULL))
	{
		DBG("Invalid sem_t provided, magic=%lx\n",((psem_t *)__sem)->magic);
		errno = EINVAL;
		return -1;
	}
	
	if(AttemptSemaphore(_SSEM(sem)))
	{
		(*__sval) = 1+sem->value;
		ReleaseSemaphore(_SSEM(sem));
	}
	else
	{
		(*__sval) = -_SSEM(sem)->ss_QueueCount;
	}
	
	return 0;
}
