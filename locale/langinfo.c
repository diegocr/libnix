
#include <stdlib.h>
#include <errno.h>
#include <proto/locale.h>
#include "locale.h"

/**
 * nl_langinfo() for AmigaOS ...
 * (C)2006 Diego Casorran <diegocr@users.sf.net>
 *
 * Info for the implementation:
 * http://www.opengroup.org/onlinepubs/009695399/functions/nl_langinfo.html
 *
 * ChangeLog:
 *   20060213: Initial Version (note thats just a little implementation,
 *       intended to be "portable", ie: date fmts to be used with strftime())
 */


/****************************************************************************/
/**
 * Stolen from langinfo.h (isnt included due conflics with Amiga's Locale)
 */
#define CODESET         0       /* codeset name */
#define D_T_FMT         1       /* string for formatting date and time */
#define D_FMT           2       /* date format string */
#define T_FMT           3       /* time format string */
#define T_FMT_AMPM      4       /* a.m. or p.m. time formatting string */
//#define AM_STR          5       /* Ante Meridian affix */
//#define PM_STR          6       /* Post Meridian affix */

/* day_, abday_, mon_, abmon_, */

#define ERA             45      /* era description segments */
#define ERA_D_FMT       46      /* era date format string */
#define ERA_D_T_FMT     47      /* era date and time format string */
#define ERA_T_FMT       48      /* era time format string */
#define ALT_DIGITS      49      /* alternative symbols for digits */

#define RADIXCHAR       50      /* radix char */
#define THOUSEP         51      /* separator for thousands */

#define YESEXPR         52      /* affirmative response expression */
#define NOEXPR          53      /* negative response expression */

//#define YESSTR          54      /* affirmative response for yes/no queries */
//#define NOSTR           55      /* negative response for yes/no queries */

#define CRNCYSTR        56      /* currency symbol */
#define D_MD_ORDER      57      /* month/day order (local extension) */

/****************************************************************************/

#ifdef __libnix__
extern struct Locale *__localevec[];
#define __global_locale __localevec[0]
#endif

#define LSTR(id)	(char *) GetLocaleStr(__global_locale, (id))

char *nl_langinfo(int nl_item)
{
	int amiga_item = (nl_item - 6);

	if(__global_locale == NULL) {
	  	errno = EFAULT;
  		return NULL;
	}

	if((amiga_item >= DAY_1) && (amiga_item <= DAY_7)) {

		return LSTR(amiga_item-DAY_1);
	}
	if((amiga_item >= ABDAY_1) && (amiga_item <= ABDAY_7)) {

		return LSTR(amiga_item-ABDAY_1);
	}
	if((amiga_item >= MON_1) && (amiga_item <= MON_12)) {

		return LSTR(amiga_item-MON_1);
	}
	if((amiga_item >= ABMON_1) && (amiga_item <= ABMON_12)) {

		return LSTR(amiga_item-ABMON_1);
	}

	switch(nl_item)
	{
		case CODESET:
		{
			char *cs = (char *)__global_locale->loc_CodeSet;

			if(cs && *cs)
				return cs;

			if((cs = getenv("LC_CTYPE")) && *cs)
				return cs;

			if((cs = getenv("LC_ALL")) && *cs)
				return cs;

			return "ANSI_X3.4-1968";
		}

		case D_T_FMT:		return "%b %a %d %k:%M:%S %Z %Y";
		case D_FMT:		return "%b %a %d";
		case T_FMT:		return "%H:%M";
		case T_FMT_AMPM:	return "%I:%M:%S %p";
		case 5 /*AM_STR*/:	return LSTR(AM_STR);
		case 6 /*PM_STR*/:	return LSTR(PM_STR);

		case ERA:		return NULL;
		case ERA_D_FMT:
		case ERA_D_T_FMT:
		case ERA_T_FMT:
		case ALT_DIGITS:	return("");

		// hmm...
		case RADIXCHAR:	return __global_locale->loc_DecimalPoint;
		case THOUSEP:	return __global_locale->loc_GroupSeparator;
		case CRNCYSTR:	return __global_locale->loc_MonCS;
		case 54:	return LSTR(YESSTR);
		case 55:	return LSTR(NOSTR);
		case YESEXPR:	return "^[yY]";
		case NOEXPR:	return "^[nN]";

		default:
			break;
	}

	errno = ENOSYS;
	return NULL;
}
