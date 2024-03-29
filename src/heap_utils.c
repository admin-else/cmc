#include "err.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *MALLOC(size_t n) {
  if (n == 0)
    ERR(ERR_MALLOC_ZERO, return NULL;);
  void *p = malloc(n);
  if (p == NULL)
    ERR(ERR_MEM, );
  return p;
}

void FREE(void *p) { free(p); }