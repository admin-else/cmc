#include <cmc/nbt.h>

#include <cmc/err.h>
#include <stdlib.h>

#include "err_macros.h"
#include <cmc/heap_utils.h>

cmc_buff *cmc_nbt_text(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

cmc_buff *cmc_nbt_dump(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

void cmc_nbt_free(cmc_nbt *tree) {
  (void)tree;
  abort(); // unimplemented
}

cmc_nbt *cmc_nbt_parse(cmc_buff *buff) {
  CMC_ERRB(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}
