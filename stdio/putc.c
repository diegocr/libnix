
#include <stdio.h>

#undef putc
int putc(int c,FILE *stream)
{
	return fputc(c,stream);
}
