#include "cmc/types/pstring.h"
#include "cmc/buff.h"
#include "cmc/err.h"
#include "../heap_utils.h"
#include <string.h>

cmc_err cmc_pack_pstring(cmc_buff_packing *buff, cmc_pstring str, size_t len) {
  // TODO: this needs utf-8 validation and proper len checking
  if(len > CMC_PSTRING_MAX) {
    return CMC_ERR_PSTRING_TOO_LONG;
  }

  return cmc_pack(buff, str, len);
}

cmc_pstring cmc_unpack_pstring(cmc_buff_unpacking *buff, size_t len, cmc_err *err) {
  if(len > CMC_PSTRING_MAX) { // MC checks for utf8 len this is temporary
    *err = CMC_ERR_PSTRING_TOO_LONG;
    return NULL;
  }

  void *addr = cmc_unpack(buff, len, err);
  if(*err != CMC_ERR_NO) {
    return NULL;
  }

  // TODO: Add proper utf validation

  cmc_pstring str = cmc_malloc(len + 1, err);
  if(*err != CMC_ERR_NO) {
    return NULL;
  }

  str[len] = '\0';
  memcpy(str, addr, len);
  return str;
}
