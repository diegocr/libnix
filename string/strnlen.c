#include <stdlib.h>
#include <string.h>

size_t strnlen(const char *s, size_t len)
{
    size_t i;
    
    if( s == NULL )
    	return 0;

    for(i = 0; i < len && s[i]; i++)
	;
    return i;
}
