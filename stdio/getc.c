
#include <stdio.h>

#undef getc
int getc(FILE *s)
{
	return fgetc(s);
}
