#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double log10(double x)
{ return IEEEDPLog10(x); }

ALIAS(log10f,log10);
ALIAS(log10l,log10);
