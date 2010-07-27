#include <stdio.h>
#include <string.h>
#include <errno.h>

#if 0
// This was buggy because stderr might be unbuffered.. [diegocr]
void perror(const char *string)
{ int err=errno;
  if(string!=NULL)
  { fputs(string,stderr);
    fputc(':',stderr);
    fputc(' ',stderr); }
  fputs(strerror(err),stderr);
  fputc('\n',stderr);
}
#else
extern char *__procname;
void perror(const char *string)
{ int err=errno;
  fprintf(stderr,"%s: %s\n",string?string:__procname,strerror(err));
}
#endif
