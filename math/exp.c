#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double exp(double x)
{ return IEEEDPExp(x); }

ALIAS(expf,exp);
ALIAS(expl,exp);
