#include <string.h>

#if 0

void *memset(void *s,int c,size_t n)
{ size_t m;
  if(n)
  { unsigned long *p=(unsigned long *)s;
    if(n>15)
    { c*=0x01010101;
      if((long)p&1)
      { *((char *)p)++=c;
        n--; }
      if((long)p&2)
      { *((short *)p)++=c;
        n-=2; }
      for(m=n/(8*sizeof(long));m;--m)
      { *p++=c; *p++=c; *p++=c; *p++=c; 
        *p++=c; *p++=c; *p++=c; *p++=c; }
      n&=(8*sizeof(long)-1);
      for(m=n/sizeof(long);m;--m)
        *p++=c;
      if((n&=sizeof(long)-1)==0) return s;
    }
    do;while(*((char *)p)++=c,--n);
  }
  return s;
}
#else

void *memset(void *s,int c,size_t n)
{ size_t m;
  if(n)
  { unsigned long *p=(unsigned long *)s;
    c*=0x01010101;
    if(n>15)
    { if((long)p&1)
      { *((char *)p)++=c;
        n--; }
      if((long)p&2)
      { *((short *)p)++=c;
        n-=2; }
      for(m=n/(8*sizeof(long));m;--m)
      { *p++=c; *p++=c; *p++=c; *p++=c; 
        *p++=c; *p++=c; *p++=c; *p++=c; }
      n&=(8*sizeof(long)-1);
      for(m=n/sizeof(long);m;--m)
        *p++=c;
      if((n&=sizeof(long)-1)==0) return s;
    }
    //do;while(*((char *)p)++=c,--n);
    switch(n)
    {
      case 15:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 14:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 13:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 12:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p) = c;
	break;
      case 11:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 10:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 9:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 8:
	*((long *)p)++ = c;
	*((long *)p) = c;
	break;
      case 7:
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 6:
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 5:
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 4:
	*((long *)p) = c;
	break;
      case 3:
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 2:
	*((short *)p) = c;
	break;
      case 1:
	*((char *)p) = c;
	break;
    }
  }
  return s;
}
#endif
