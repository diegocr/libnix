#include <stdlib.h>

extern char *
  readpassphrase(const char *prompt, char *buf, size_t bufsiz, int flags);

char *getpass_r(const char *prompt,char *password,size_t buflen)
{
	return(readpassphrase(prompt, password, buflen, 0));
}
