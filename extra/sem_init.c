
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include "psem.h"
#include <string.h>	/* memcpy */

int sem_init (sem_t *__sem, int __pshared, unsigned int __value)
{
	psem_t *sem;
	
	if((__sem == NULL) || (__value > SEM_VALUE_MAX))
	{
		errno = EINVAL;
		return -1;
	}
	
	sem = psem_init(NULL,__pshared,__value);
	
	memcpy(__sem,sem,sizeof(psem_t));
	
	return 0;
}

int sem_destroy (sem_t *__sem)
{
	psem_t *sem = sem2psem(__sem);
	
	ENTER();
	POINTER(sem);
	
	if(_PSEM_INVALID(sem))
	{
		DBG("Invalid sem_t provided, magic=%lx\n",((psem_t *)__sem)->magic);
		errno = EINVAL;
		return -1;
	}
	
	psem_destroy(sem);
	memset(__sem,_PSEM_ERASEDBIT,sizeof(*__sem));
	
	LEAVE();
	return 0;
}
