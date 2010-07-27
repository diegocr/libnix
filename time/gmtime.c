#include <time.h>

/* Rules for leap-years:
 * 1. every 4th year is a leap year
 * 2. every 100th year is none
 * 3. every 400th is one
 * 4. 1900 was none, 2000 is one
 */

extern int __dstflag;
extern struct tm *gmtime_r(const time_t *t,struct tm *utim);

static char monthtable[]=
{ 31,29,31,30,31,30,31,31,30,31,30 };

struct tm *gmtime(const time_t *t)
{
  static struct tm utim;
  return gmtime_r(t,&utim);
}

struct tm *gmtime_r(const time_t *t,struct tm *utim)
{
  signed long tim;
  int leapday=0,leapyear=0,i;
  tim=*t;
  utim->tm_sec=tim%60;
  tim/=60;
  utim->tm_min=tim%60;
  tim/=60;
  utim->tm_hour=tim%24;
  tim=tim/24+719162;
  utim->tm_wday=(tim+1)%7;
  utim->tm_year=tim/146097*400-1899;
  tim%=146097;
  if(tim>=145731)
  { leapyear++; /* The day is in one of the 400th */
    if(tim==146096)
    { tim--; /* Be careful: The last of the 4 centuries is 1 day longer */
      leapday++; }
  }
  utim->tm_year+=tim/36524*100;
  tim%=36524;
  if(tim>=36159)
    leapyear--; /* The day is in one of the 100th */
  utim->tm_year+=tim/1461*4;
  tim%=1461;
  if(tim>=1095)
  { leapyear++; /* The day is in one of the 4th */
    if(tim==1460)
    { tim--; /* Be careful: The 4th year is 1 day longer */
      leapday++; }
  }
  utim->tm_year+=tim/365;
  tim=tim%365+leapday;
  utim->tm_yday=tim;
  if(!leapyear&&tim>=31+28)
    tim++; /* add 1 for 29-Feb if no leap year */
  for(i=0;i<11;i++)
    if(tim<monthtable[i])
      break;
    else
      tim-=monthtable[i];
  utim->tm_mon=i;
  utim->tm_mday=tim+1;
  utim->tm_isdst=__dstflag;
  return utim;
}

/*
 * 719162 number of days between 1.1.1 and 1.1.1970
 *        if the calendar would go so far which it doesn't :-)
 *        this is true for all of the following.
 * 146097 number of days from 1.1.1 to 1.1.401
 * 145731 number of days from 1.1.1 to 1.1.400
 *  36524 number of days from 1.1.1 to 1.1.101
 *  36159 number of days from 1.1.1 to 1.1.100
 *   1461 number of days from 1.1.1 to 1.1.5
 *   1095 number of days from 1.1.1 to 1.1.4
 */
