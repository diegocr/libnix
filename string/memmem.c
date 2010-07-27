#include <stdlib.h>
#include <string.h>

#define __INLINESTUB
#include "memcmp.c"

void *memmem (const void *a, size_t a_len, const void *b,size_t b_len)
{
  const char *start = (const char *) a;
  const char *const end = start + a_len - b_len + 1;
  
  if(b_len == 0)
    return (void *) a;
  
  if((signed)b_len < 1 || !a || !b || (a_len < b_len))
    return NULL;
  
  while( start < end )
  {
    if(!__memcmp((const unsigned char *)start,(const unsigned char *)b,b_len))
      return (void *) start;
    
    ++start;
  }
  
  return NULL;
}
