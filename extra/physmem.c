#include <stdlib.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include <exec/execbase.h>

#define ITERATE_LIST(list, type, node)				\
	for (node = (type)((struct List *)(list))->lh_Head;	\
		((struct MinNode *)node)->mln_Succ;		\
			node = (type)((struct MinNode *)node)->mln_Succ)

/* Return the total amount of physical memory. */
double physmem_total (void)
{
  struct MemHeader *mh;
  ULONG result = 0;
  
  Forbid();
  
  ITERATE_LIST(&SysBase->MemList, struct MemHeader *, mh) {
    result += (mh->mh_Upper-mh->mh_Lower)/1024;
  }
  Permit();
  
  return((double)result * 1024.0);
}

/* Return the amount of physical memory available. */
double physmem_available (void)
{
  return (double)AvailMem(MEMF_FAST);
}
