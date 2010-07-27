#if 0
#include <proto/exec.h>
#include <string.h>

void *memcpy(void *s1,const void *s2,size_t n)
{
  CopyMem((APTR)s2,s1,n); return s1;
}
#else
# include <stdlib.h>
# define bcopy __bCopy
static inline
# include "../misc/bcopy.c"
void *memcpy(void *s1,const void *s2,size_t n)
{
  bcopy(s2,s1,n);
  return s1;
}
#endif
