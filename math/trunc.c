#include <proto/mathieeedoubbas.h>
#include "stabs.h"

double trunc(double x)
{ return IEEEDPFloor(x); }

ALIAS(truncf,trunc);
ALIAS(truncl,trunc);
