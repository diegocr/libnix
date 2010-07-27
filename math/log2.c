#include <proto/mathieeedoubtrans.h>
#include "stabs.h"

double log2(double x) {
  return (IEEEDPLog(x) * 1.44269504088896340736);
}

ALIAS(log2f,log2);
ALIAS(log2l,log2);
