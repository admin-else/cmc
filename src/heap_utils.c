#include "cmc/err.h"
#include <stdlib.h>

void *cmc_malloc(unsigned long len, cmc_err *err) {
  if (len == 0) {
    *err = CMC_ERR_MALLOC_ZERO;
    return NULL;
  }
  void *p_data = malloc(len);
  if (p_data == NULL) {
    *err = CMC_ERR_MALLOC_FAIL;
    return NULL;
  }
  return p_data;
}

void *cmc_realloc(void *in_p, unsigned long len, cmc_err *err) {
  if (len == 0) {
    *err = CMC_ERR_RELLOC_ZERO; // althought this is inteded behaivior most
                                // likey you should have just used free
    return NULL;
  }
  void *p_data = realloc(in_p, len);
  if (p_data == NULL) {
    *err = CMC_ERR_RELLOC_FAIL;
    return NULL;
  }
  return p_data;
}

void cmc_free(void *p_data) { free(p_data); }
