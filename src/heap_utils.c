#include <cmc/heap_utils.h>

#include <cmc/err.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmc/buff.h"
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

void *cmc_malloc_packet_array(cmc_buff *buff, size_t size) {
  /*
   This function is supposed to catch weird allocation request.
   This is achived by checking how much memory is left in the buffer to read and
   comparing that to the amount of that would have to be allocated.
  */
  if (buff->length - buff->position < size) {
    CMC_ERRB(CMC_ERR_ARRAY_LEN_TOO_BIG, return NULL;);
  }

  return cmc_malloc(size, buff->err);
}

void cmc_free(void *p) {
  free(p);
}
