#include <string.h>

void bzero(void *b,size_t n)
{ size_t m;
  if(!n)
    return;
  if(n>15)
  { if((long)b&1)
    { *((char *)b)++=0;
      n--; }
    if((long)b&2)
    { *((short *)b)++=0;
      n-=2; }
    for(m=n/(8*sizeof(long));m;--m)
    { *((long *)b)++=0; *((long *)b)++=0; *((long *)b)++=0; *((long *)b)++=0;
      *((long *)b)++=0; *((long *)b)++=0; *((long *)b)++=0; *((long *)b)++=0; }
    n&=8*sizeof(long)-1;
    for(m=n/sizeof(long);m;--m)
      *((long *)b)++=0;
    if((n&=sizeof(long)-1)==0) return;
  }
  //do;while(*((char *)b)++=0,--n);
  switch(n)
  {
    case 15:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((short *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 14:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((short *)b) = 0;
      break;
    case 13:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 12:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((long *)b) = 0;
      break;
    case 11:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((short *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 10:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((short *)b) = 0;
      break;
    case 9:
      *((long *)b)++ = 0;
      *((long *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 8:
      *((long *)b)++ = 0;
      *((long *)b) = 0;
      break;
    case 7:
      *((long *)b)++ = 0;
      *((short *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 6:
      *((long *)b)++ = 0;
      *((short *)b) = 0;
      break;
    case 5:
      *((long *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 4:
      *((long *)b) = 0;
      break;
    case 3:
      *((short *)b)++ = 0;
      *((char *)b) = 0;
      break;
    case 2:
      *((short *)b) = 0;
      break;
    case 1:
      *((char *)b) = 0;
      break;
  }
}
