#ifndef mc68000

#include <string.h>

char *strcat(char *s1,const char *s2)
{ char *s=s1;

  do;while(*s++); --s; do;while((*s++=*s2++)); return s1;
}

#else

asm(
"	.globl	_strcat;"
"_strcat:;"
"	movml	sp@(4:W),d0/a0;"
"	movel	d0,a1;"
"L2:	tstb	a1@+;"
"	jne	L2;"
"	subql	#1,a1;"
"L1:	moveb	a0@+,a1@+;"
"	jne	L1;"
"	rts;"
);

#endif
