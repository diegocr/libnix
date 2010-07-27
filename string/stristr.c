#include <string.h>
#include <proto/utility.h>
#include "stabs.h"

char *stristr(const char *a,const char *b)
{
	if(a&&b)
	{
		int l = strlen(b);

		while( *a )
		{
			if(!Strnicmp(a, b, l))
				return((char *)a);
			a++;
		}
	}

	return NULL;
}

ALIAS(strcasestr,stristr);

