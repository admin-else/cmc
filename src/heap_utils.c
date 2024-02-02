#include "err.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_ACTION
void *MALLOC(size_t n) {
  if (n == 0)
    ERR(ERR_MALLOC_ZERO);
  void *p = malloc(n);
  if (p == NULL)
    ERR(ERR_MEM);
  return p;
}

void FREE(void *p) { free(p); }