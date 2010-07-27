/*							cbrt.c
 *
 *	Cube root
 *
 *
 *
 * SYNOPSIS:
 *
 * float x, y, cbrt();
 *
 * y = cbrt( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the cube root of the argument, which may be negative.
 *
 * Range reduction involves determining the power of 2 of
 * the argument.  A polynomial of degree 2 applied to the
 * mantissa, and multiplication by the cube root of 1, 2, or 4
 * approximates the root to within about 0.1%.  Then Newton's
 * iteration is used to converge to an accurate result.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,1e38      100000      7.6e-8      2.7e-8
 *
 */
/*							cbrt.c  */

/*
Cephes Math Library Release 2.2:  June, 1992
Copyright 1984, 1987, 1988, 1992 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"
#include "stabs.h"

static float CBRT2 = 1.25992104989487316477f;
static float CBRT4 = 1.58740105196819947475f;


#ifdef ANSIC
#ifndef __libnix__
float frexp(float, int *), ldexp(float, int);
#else
#ifndef frexp
# define frexp __frexp
static __inline__ double frexp(double x,int *p) {
  int neg,j=neg=0;
  if(x<0){ x=-x; neg=1; }
  if(x>0) do { ++j; x/=2; } while( x > 0 );
  else if(x<0.5&&x!=0) do { --j; x*=2; } while(x<0.5);
  *p = j;if(neg) x=-x;
  return x;
}
#endif
#ifndef ldexp
#define ldexp __ldexp
#define MANT_MASK 0x800FFFFF    /* Mantissa extraction mask     */
#define ZPOS_MASK 0x3FF00000    /* Positive # mask for exp = 0  */
#define ZNEG_MASK 0x3FF00000    /* Negative # mask for exp = 0  */

#define EXP_MASK 0x7FF00000     /* Mask for exponent            */
#define EXP_SHIFTS 20           /* Shifts to get into LSB's     */
#define EXP_BIAS 1023           /* Exponent bias                */

union dtol { double dval; int ival[2]; };

static __inline__ double ldexp (double x,int n) {
  union dtol number;int *iptr, cn;
  if(x == 0.0)return(0.0);
  number.dval = x;
  iptr = &number.ival[0];
  cn = (((*iptr) & EXP_MASK) >> EXP_SHIFTS) - EXP_BIAS;
  *iptr &= ~EXP_MASK;
  n += EXP_BIAS;
  *iptr |= ((n + cn) << EXP_SHIFTS) & EXP_MASK;
  return (number.dval);
}
#endif /* ldexp */
#endif /* __libnix__ */

float cbrt( float xx )
#else
float frexp(), ldexp();

float cbrt(xx)
double xx;
#endif
{
int e, rem, sign;
float x, z;

x = xx;
if( x == 0 )
	return( 0.0f );
if( x > 0 )
	sign = 1;
else
	{
	sign = -1;
	x = -x;
	}

z = x;
/* extract power of 2, leaving
 * mantissa between 0.5 and 1
 */
x = frexp( x, &e );

/* Approximate cube root of number between .5 and 1,
 * peak relative error = 9.2e-6
 */
x = (((-0.13466110473359520655053f * x
      + 0.54664601366395524503440f ) * x
      - 0.95438224771509446525043f ) * x
      + 1.1399983354717293273738f  ) * x
      + 0.40238979564544752126924f;

/* exponent divided by 3 */
if( e >= 0 )
	{
	rem = e;
	e /= 3;
	rem -= 3*e;
	if( rem == 1 )
		x *= CBRT2;
	else if( rem == 2 )
		x *= CBRT4;
	}


/* argument less than 1 */

else
	{
	e = -e;
	rem = e;
	e /= 3;
	rem -= 3*e;
	if( rem == 1 )
		x /= CBRT2;
	else if( rem == 2 )
		x /= CBRT4;
	e = -e;
	}

/* multiply by power of 2 */
x = ldexp( x, e );

/* Newton iteration */
x -= ( x - (z/(x*x)) ) * 0.333333333333f;

if( sign < 0 )
	x = -x;
return(x);
}

ALIAS(cbrtf,cbrt);
ALIAS(cbrtl,cbrt);
