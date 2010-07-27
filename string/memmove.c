#include <string.h>
#include <stdlib.h>

#define bcopy __bCopy
static inline
#include "../misc/bcopy.c"

void *memmove(void *s1,const void *s2,size_t n)
{
  bcopy(s2,s1,n);
  return s1;
}
