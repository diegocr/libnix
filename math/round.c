#include <proto/mathieeedoubbas.h>
#include "stabs.h"

double round(double x)
{ return(( x > 0.0 )?IEEEDPFloor(x + 0.5):IEEEDPCeil(x - 0.5)); }

ALIAS(roundf,round);
ALIAS(roundl,round);
