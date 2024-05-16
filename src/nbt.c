#include <cmc/nbt.h>

#include "err_macros.h"

#include <cmc/err.h>

cmc_nbt *cmc_nbt_parse(cmc_buff *buff, cmc_err_extra *err) {
  (void)buff;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

cmc_buff *cmc_nbt_text(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

cmc_buff *cmc_nbt_dump(const cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, return NULL;);
}

void cmc_nbt_free(cmc_nbt *tree, cmc_err_extra *err) {
  (void)tree;
  CMC_ERR(CMC_ERR_NOT_IMPLEMENTED_YET, );
}
