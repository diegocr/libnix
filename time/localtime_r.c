#include <time.h>

extern long __gmtoffset;
#if __GNUC__ < 3
extern struct tm *gmtime_r(const time_t *t,struct tm *utim);
#endif

#undef localtime_r
struct tm *localtime_r(const time_t *t,struct tm *result)
{ time_t ti=*t;
  ti-=__gmtoffset*60;
  return gmtime_r(&ti,result);
}
