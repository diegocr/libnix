#include <stdlib.h>

extern char *
  readpassphrase(const char *prompt, char *buf, size_t bufsiz, int flags);

#ifndef _PASSWORD_LEN
# define _PASSWORD_LEN 127
#endif

char * getpass(const char *prompt)
{
	static char buf[_PASSWORD_LEN + 1];

	return(readpassphrase(prompt, buf, sizeof(buf), 0));
}
