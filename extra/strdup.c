#include <stdlib.h>
#include <string.h>
#include <strsup.h>

#if 0

char *strdup(const char *s)
{ char *s1=malloc(strlen_plus_one(s));
  if (s1)
    strcpy(s1,s);
  return s1;
}

#else
extern size_t strnlen(const char *s, size_t len);

static __inline__ char * __strndup(const char *s, size_t len)
{
  char * result;
  if((result = malloc(len + 1)) != NULL)
  {
    bcopy( s, result, len);
  }
  return result;
}

char * strndup(const char *s, size_t sz)
{
  if( s == NULL )
    return NULL;
  return __strndup(s,strnlen(s,sz)+1);
}

char *strdup(const char *s)
{
  if(s == NULL)
    return NULL;
  return __strndup(s,strlen_plus_one(s));
}

#endif
