
/**
 * POSIX Semaphores Emulation for AmigaOS
 * Copyright (C)2009 Diego Casorran
 * 
 * Public Domain as long this copyright notice is left unchanged.
 * 
 * Implementation conforms to POSIX.1-2008 where possible.
 */

#include "psem.h"
#include <fcntl.h>	/* O_CREAT O_EXCL */
#include <stdarg.h>	/* va_list */
#include <string.h>	/* strlen */

sem_t *sem_open (const char *__name, int __oflag, ...)
{
	unsigned char name[_PSEM_NAME_MAXLENGTH];
	struct SignalSemaphore *ssem;
	psem_t *psem = NULL;
	mode_t mode = 0;
	unsigned value = 0;
	sem_t *rc = SEM_FAILED;
	
	if(strlen(__name) > (_PSEM_NAME_MAXLENGTH-20))
	{
		errno = ENAMETOOLONG;
		return(SEM_FAILED);
	}
	
	errno = 0;
	psem_name(__name,name,sizeof(name)-1);
	
	Forbid();
	if((ssem = FindSemaphore((STRPTR)name)))
	{
		if(((psem_t *)ssem)->magic == _PSEM_MAGIC)
			psem = (psem_t *)ssem;
	}
	Permit();
	
	if((ssem != NULL) && (psem == NULL))
	{
		errno = ENOSPC;
	}
	else if(__oflag & O_CREAT)
	{
		if((__oflag & O_EXCL) && (psem != NULL))
		{
			errno = EEXIST;
		}
		else if( psem == NULL )
		{
			va_list ap;
			
			va_start(ap, __oflag);
			mode = va_arg(ap, int);
			value = va_arg(ap, unsigned int);
			va_end(ap);
			
			if( value > SEM_VALUE_MAX )
			{
				errno = EINVAL;
			}
			else if((psem = psem_init(__name,0,value))==(psem_t *)SEM_FAILED)
			{
				errno = ENOMEM;
			}
		}
		else if(psem->owner != (unsigned)FindTask(NULL))
		{
			/**
			 * Make a dup of this psem to track it down and be sure
			 * it's properly released on exit.
			 */
			
			psem_t * semdup;
			
			if((semdup = AllocVec(sizeof(psem_t),MEMF_PUBLIC)))
			{
				memcpy( semdup, psem, sizeof(psem_t));
				semdup->flags |= SEMF_OPENDUP;
				
				Forbid();
				psem_list_add(semdup);
				Permit();
			}
		}
	}
	else if( psem == NULL )
	{
		errno = ENOENT;
	}
	
	if( errno == 0 )
	{
		rc = (sem_t *) psem;
	}
	
	return(rc);
}
