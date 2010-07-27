#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double exp2(double x)
{ return (IEEEDPExp(x) * 0.693147180559945); }

ALIAS(exp2f,exp2);
ALIAS(exp2l,exp2);
