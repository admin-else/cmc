#include <cmc/heap_utils.h>

#include <cmc/err.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "err_macros.h"

void *cmc_malloc(size_t n, cmc_err_extra *err) {
  if (n == 0)
    CMC_ERR(CMC_ERR_MALLOC_ZERO, return NULL;);
  void *p = malloc(n);
  if (p == NULL)
    CMC_ERR(CMC_ERR_MEM, return NULL;);
  return p;
}

void *cmc_realloc(void *p, size_t n, cmc_err_extra *err) {
  if (n == 0)
    CMC_ERR(CMC_ERR_REALLOC_ZERO, return NULL;);
  p = realloc(p, n);
  if (p == NULL)
    CMC_ERR(CMC_ERR_MEM, return NULL;);
  return p;
}
