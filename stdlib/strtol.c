#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

signed long strtol(const char *nptr,char **endptr,int base)
{ const char *p=nptr;
  char *q;
  unsigned long r;
  if(!(nptr && *nptr))
  { errno=EINVAL;
    return 0;
  }
  while(isspace(*p))
    p++;
  r=strtoul(p,&q,base);
  if(endptr!=NULL)
  { if(q==p)
      *endptr=(char *)nptr;
    else
      *endptr=q;
  }
  if(*p=='-')
  { if((signed long)r>0)
    { errno=ERANGE;
      return LONG_MIN; }
    else
      return r;
  }else
  { if((signed long)r<0)
    { errno=ERANGE;
      return LONG_MAX; }
    else
      return r;
  }
}
