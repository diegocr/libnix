#include <time.h>
#include <limits.h>

char *asctime_r(const struct tm *t,char *buffer)
{ strftime(buffer,INT_MAX,"%C\n",t);
  return buffer;
}

#if __GNUC__ < 3
extern struct tm *localtime_r(const time_t *t,struct tm *result);
#endif

#undef ctime_r
char *ctime_r(const time_t *t,char *buf)
{ struct tm tmx;
  return asctime_r(localtime_r(t,&tmx),buf);
}
