#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double hypot(double x,double y)
{ register double v = (x*x+y*y);
  return IEEEDPSqrt(v);
}

ALIAS(hypotf,hypot);
ALIAS(hypotl,hypot);
