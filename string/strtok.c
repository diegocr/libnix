#include <string.h>

#if 0 // libnix < 3.x
char *strtok(char *s1,const char *s2)
{ static char *t;
  if(s1)
    t=s1;
  else
    s1=t;
  s1+=strspn(s1,s2);
  if(*s1=='\0')
    return (char *)0;
  t=s1;
  t+=strcspn(s1,s2);
  if(*t!='\0')
    *t++='\0';
  return s1;
}
#else
extern char *strtok_r(char *s1,const char *s2,char **t);
static char *t = NULL;

char *strtok(char *s1,const char *s2)
{
  return strtok_r(s1,s2,&t);
}
#endif
