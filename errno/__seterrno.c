#include <errno.h>
#include <dos/dosasl.h>
#include <proto/dos.h>

/* Table to convert amigados error messages to unix ones */

#if 0 // libnix < 3.x

static long _errortable[] = {
  ERROR_NO_FREE_STORE		,ENOMEM,
  ERROR_TASK_TABLE_FULL		,EPROCLIM,
/*ERROR_BAD_TEMPLATE
  ERROR_BAD_NUMBER
  ERROR_REQUIRED_ARG_MISSING
  ERROR_KEY_NEEDS_ARG*/
  ERROR_TOO_MANY_ARGS		,E2BIG,
/*ERROR_UNMATCHED_QUOTES*/
  ERROR_LINE_TOO_LONG		,E2BIG,
/*ERROR_FILE_NOT_OBJECT
  ERROR_INVALID_RESIDENT_LIBRARY
  ERROR_NO_DEFAULT_DIR*/
  ERROR_OBJECT_IN_USE		,ETXTBSY,
  ERROR_OBJECT_EXISTS		,EEXIST,
  ERROR_DIR_NOT_FOUND		,ENOENT,
  ERROR_OBJECT_NOT_FOUND	,ENOENT,
  ERROR_BAD_STREAM_NAME		,ENAMETOOLONG,
  ERROR_OBJECT_TOO_LARGE	,EFBIG,
  ERROR_ACTION_NOT_KNOWN	,ENODEV,
/*ERROR_INVALID_COMPONENT_NAME
  ERROR_INVALID_LOCK
  ERROR_OBJECT_WRONG_TYPE*/
  ERROR_DISK_NOT_VALIDATED	,EBUSY,
  ERROR_DISK_WRITE_PROTECTED	,EROFS,
  ERROR_RENAME_ACROSS_DEVICES	,EXDEV,
  ERROR_DIRECTORY_NOT_EMPTY	,ENOTEMPTY,
  ERROR_TOO_MANY_LEVELS		,ENAMETOOLONG,
  ERROR_DEVICE_NOT_MOUNTED	,ENXIO,
  ERROR_SEEK_ERROR		,ESPIPE,
/*ERROR_COMMENT_TOO_BIG*/
  ERROR_DISK_FULL		,ENOSPC,
  ERROR_DELETE_PROTECTED	,EACCES,
  ERROR_WRITE_PROTECTED		,EACCES,
  ERROR_READ_PROTECTED		,EACCES,
  ERROR_NOT_A_DOS_DISK		,EFTYPE,
  ERROR_NO_DISK			,ENXIO,
/*ERROR_NO_MORE_ENTRIES
  ERROR_IS_SOFT_LINK
  ERROR_OBJECT_LINKED*/
  ERROR_BAD_HUNK		,ENOEXEC,
/*ERROR_NOT_IMPLEMENTED
  ERROR_RECORD_NOT_LOCKED
  ERROR_LOCK_COLLISION
  ERROR_LOCK_TIMEOUT
  ERROR_UNLOCK_ERROR
  ERROR_BUFFER_OVERFLOW
  ERROR_BREAK*/
  ERROR_NOT_EXECUTABLE		,EACCES,
  0				,EPERM
};
  
void __seterrno(void)
{ long amigaerror=IoErr(),*ptr=_errortable,err;

  while((err=*ptr++)&&(err!=amigaerror))
    ++ptr;
  errno=*ptr;
}
#else

void __seterrno(void)
{
	static const struct { UBYTE io_err; UBYTE errno; } map_table[] =
	{
		{ ERROR_NO_FREE_STORE,			ENOMEM },
		{ ERROR_TASK_TABLE_FULL,		ENOMEM },
		{ ERROR_BAD_TEMPLATE,			EINVAL },
		{ ERROR_BAD_NUMBER,			EINVAL },
		{ ERROR_REQUIRED_ARG_MISSING,		EINVAL },
		{ ERROR_KEY_NEEDS_ARG,			EINVAL },
		{ ERROR_TOO_MANY_ARGS,			EINVAL },
		{ ERROR_UNMATCHED_QUOTES,		EINVAL },
		{ ERROR_LINE_TOO_LONG,			ENAMETOOLONG },
		{ ERROR_FILE_NOT_OBJECT,		ENOEXEC },
		{ ERROR_OBJECT_IN_USE,			EBUSY },
		{ ERROR_OBJECT_EXISTS,			EEXIST },
		{ ERROR_DIR_NOT_FOUND,			ENOENT },
		{ ERROR_OBJECT_NOT_FOUND,		ENOENT },
		{ ERROR_BAD_STREAM_NAME,		EINVAL },
		{ ERROR_OBJECT_TOO_LARGE,		EFBIG },
		{ ERROR_ACTION_NOT_KNOWN,		ENOSYS },
		{ ERROR_INVALID_COMPONENT_NAME,		EINVAL },
		{ ERROR_INVALID_LOCK,			EBADF },
		{ ERROR_OBJECT_WRONG_TYPE,		EFTYPE },
		{ ERROR_DISK_NOT_VALIDATED,		EROFS },
		{ ERROR_DISK_WRITE_PROTECTED,		EROFS },
		{ ERROR_RENAME_ACROSS_DEVICES,		EXDEV },
		{ ERROR_DIRECTORY_NOT_EMPTY,		ENOTEMPTY },
		{ ERROR_TOO_MANY_LEVELS,		ENAMETOOLONG },
		{ ERROR_DEVICE_NOT_MOUNTED,		ENXIO },
		{ ERROR_COMMENT_TOO_BIG,		ENAMETOOLONG },
		{ ERROR_DISK_FULL,			ENOSPC },
		{ ERROR_DELETE_PROTECTED,		EACCES },
		{ ERROR_WRITE_PROTECTED,		EACCES },
		{ ERROR_READ_PROTECTED,			EACCES },
		{ ERROR_NOT_A_DOS_DISK,			EFTYPE },
		{ ERROR_NO_DISK,			EACCES },
		{ ERROR_IS_SOFT_LINK,			EFTYPE },
		{ ERROR_BAD_HUNK,			ENOEXEC },
		{ ERROR_NOT_IMPLEMENTED,		ENOSYS },
		{ ERROR_LOCK_COLLISION,			EACCES },
#if 0
		{ ERROR_BREAK,				EINTR },
		{ ERROR_NOT_EXECUTABLE,			ENOEXEC }
#endif
	};
	
	LONG io_err = IoErr();
	
	unsigned int i, table_size = (sizeof(map_table) / sizeof(map_table[0]));

	errno = EIO;

	for(i = 0 ; i < table_size ; i++)
	{
		if((LONG)map_table[i].io_err == io_err)
		{
			errno = (int) map_table[i].errno;
			break;
		}
	}
}
#endif
