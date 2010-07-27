#include <stdlib.h>

void *memrchr(const void *s, int c, size_t n)
{
  const unsigned char *p = s;
  const unsigned char *q = s;

  p += n - 1;

  while (p >= q) {
    if (*p == (unsigned char)c)
      return (void *)p;
    p--;
  }

  return NULL;
}
