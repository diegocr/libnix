
#include <math.h>
#include "stabs.h"

#if defined(vax)||defined(tahoe)

// Deal with different ways to concatenate in cpp
#  ifdef __STDC__
#    define	cat3(a,b,c) a ## b ## c
#  else
#    define	cat3(a,b,c) a/**/b/**/c
#  endif

// Deal with vax/tahoe byte order issues
#  ifdef vax
#    define	cat3t(a,b,c) cat3(a,b,c)
#  else
#    define	cat3t(a,b,c) cat3(a,c,b)
#  endif

#  define vccast(name) (*(const double *)(cat3(name,,x)))

#  define vc(name, value, x1,x2,x3,x4, bexp, xval) \
	const static long cat3(name,,x)[] = {cat3t(0x,x1,x2), cat3t(0x,x3,x4)};

#  define ic(name, value, bexp, xval) ;

#else //  vax or tahoe

   // Hooray, we have an IEEE machine
#  undef vccast
#  define vc(name, value, x1,x2,x3,x4, bexp, xval) ;

#  define ic(name, value, bexp, xval) \
	static const double name = value;

#endif /* defined(vax)||defined(tahoe) */

vc (L, 4503599627370496.0E0, 0000, 5 c00, 0000, 0000, 55, 1.0)	/* 2**55 */
  ic (L, 4503599627370496.0E0, 52, 1.0)	/* 2**52 */
#ifdef vccast
#define	L	vccast(L)
#endif
#if defined(vax)||defined(tahoe)	/* VAX D format */
#include <errno.h>
     static const unsigned short msign = 0x7fff, mexp = 0x7f80;
     static const short prep1 = 57, gap = 7, bias = 129;
     static const double novf = 1.7E38, nunf = 3.0E-39, zero = 0.0;
#else /* defined(vax)||defined(tahoe) */
     static const unsigned short msign = 0x7fff, mexp = 0x7ff0;
     static const short prep1 = 54, gap = 4, bias = 1023;
     static const double novf = 1.7E308, nunf = 3.0E-308, zero = 0.0;
#endif /* defined(vax)||defined(tahoe) */


static __inline__ double __copysign (double x, double y)
{
#ifdef national
  unsigned short *px = (unsigned short *) &x + 3,
    *py = (unsigned short *) &y + 3;
#else /* national */
  unsigned short *px = (unsigned short *) &x, *py = (unsigned short *) &y;
#endif /* national */

#if defined(vax)||defined(tahoe)
  if ((*px & mexp) == 0)
    return (x);
#endif /* defined(vax)||defined(tahoe) */

  *px = (*px & msign) | (*py & ~msign);
  return (x);
}

double copysign (double x, double y)
{ return __copysign(x,y); }

double
rint (double x)
{
  double s, t;
  const double one = 1.0;

  if (x != x)			/* NaN */
    return (x);

  if(__copysign (x, one) >= L)	/* already an integer */
    return (x);
  s = __copysign (L, x);
  t = x + s;			/* x+s rounded to integer */
  return (t - s);
}

ALIAS(rintf,rint);
ALIAS(rintl,rint);

ALIAS(copysignf,copysign);
ALIAS(copysignl,copysign);
