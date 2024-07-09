#include "cmc/err.h"
#include <stdlib.h>

void *cmc_malloc(unsigned long len, cmc_err *err) {
  if(len == 0) {
    *err = CMC_ERR_MALLOC_FAIL;
    return NULL;
  }
  void *p = malloc(len);
  if(p == NULL) {
    *err = CMC_ERR_MALLOC_FAIL;
    return NULL;
  }
  return p;
}
