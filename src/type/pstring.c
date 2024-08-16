#include <cmc/type/pstring.h>
#include "../heap_utils.h"
#include "cmc/buff.h"
#include "cmc/err.h"
#include <string.h>

cmc_err cmc_pstring_pack(cmc_buff *buff, cmc_pstring str, size_t len) {
  // TODO: this needs utf-8 validation and proper len checking
  if (len > CMC_PSTRING_MAX) {
    return CMC_ERR_PSTRING_TOO_LONG;
  }

  return cmc_pack(buff, str, len);
}

cmc_pstring cmc_pstring_unpack(cmc_span *buff, size_t len, cmc_err *err) {
  if (len > CMC_PSTRING_MAX) { // MC checks for utf8 len this is temporary
    *err = CMC_ERR_PSTRING_TOO_LONG;
    return NULL;
  }

  void *addr = cmc_unpack(buff, len, err);
  if (*err != CMC_ERR_NO) {
    return NULL;
  }

  // TODO: Add proper utf validation

  cmc_pstring str = cmc_malloc(len + 1, err);
  if (*err != CMC_ERR_NO) {
    return NULL;
  }

  str[len] = '\0'; // I dont like this and i should change this but i probably never will :( i mean null terminator
  memcpy(str, addr, len);
  return str;
}
