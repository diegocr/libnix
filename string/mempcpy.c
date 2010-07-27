#include <string.h>
#include <stdlib.h>

#define bcopy __bCopy
static __inline__
#include "../misc/bcopy.c"

void *mempcpy(void *s1,const void *s2,size_t n)
{
  bcopy(s2,s1,n);
  return (char *)s1 + n;
}
