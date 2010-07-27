
#include "socketnix.h"

#undef inet_addr

/*
 * Ascii internet address interpretation routine.
 * The value returned is in network order.
 */
u_long inet_addr(const unsigned char *cp)
{
  struct in_addr val;

  if (inet_aton(cp, &val))
    return (val.s_addr);
  return (u_long)-1;
}
