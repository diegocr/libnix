
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include <proto/dos.h>
#include "psem.h"

int sem_timedwait (sem_t *__sem, const struct timespec *__abstime)
{
	psem_t *sem = sem2psem(__sem);
	time_t sec;
	
	if(_PSEM_INVALID(sem))
	{
		DBG("Invalid sem_t provided, magic=%lx\n",((psem_t *)__sem)->magic);
		errno = EINVAL;
		return -1;
	}
	
	if(psem_trywait(sem) == 0)
		return 0;
	
	sec = (time(NULL) - __abstime->ts_sec);
	
	while(sec > 0)
	{
		Delay(TICKS_PER_SECOND);
		
		if(psem_trywait(sem) == 0)
			return 0;
		
		sec--;
	}
	
	errno = ETIMEDOUT;
	return -1;
}
