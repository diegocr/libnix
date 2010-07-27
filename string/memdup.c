#include <stdlib.h>
#include <string.h>

/**
 * This is NOT the standard's memdup function - due the '\0'
 * ...no side effects, though.
 */

void *memdup(const void *input, size_t size)
{
	char *out;

	if(input == NULL )
	  return NULL;

	if((signed)size < 1)
	  size = strlen((char *)input);

	if((out = malloc(size+2)) != NULL)
	{
		memcpy(out, input, size);
		out[size] = '\0';
	}

	return((void *) out );
}
