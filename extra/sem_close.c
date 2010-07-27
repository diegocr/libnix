
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include "psem.h"
#include <string.h>	/* memset */

int sem_close (sem_t *__sem)
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
	
	if(!(sem->flags & SEMF_UNNAMED))
	{
		psem_destroy(sem);
		memset(__sem,_PSEM_ERASEDBIT,sizeof(*__sem));
	}
	
	LEAVE();
	return(0);
}
