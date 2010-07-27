#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double sqrt(double x)
{ return IEEEDPSqrt(x); }

ALIAS(sqrtf,sqrt);
ALIAS(sqrtl,sqrt);
