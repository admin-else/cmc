#include "cmc/err_macros.h"
#include <cmc/type/varnums.h>
#include <cmc/buff.h>
#include <cmc/err.h>
#include <cmc/type/c_types.h>

#define VARNUM_DATA_BITMASK (uint8_t)(0x7F)
#define VARNUM_SIGNAL_BIT (uint8_t)(0x80)
#define MAX_VARINT_SIZE_BYTES (int)5
#define MAX_VARLONG_SIZE_BYTES (int)10
#define VARNUM_DATA_BITS_LEN (int)7

cmc_err cmc_varint_pack(cmc_buff *buff, cmc_varint number) {
  assert(buff);
  uint32_t number_u = number;
  cmc_err err = CMC_ERR_NO;
  for (int i = 0; i < MAX_VARINT_SIZE_BYTES; ++i) {
    uint8_t byte = number_u & VARNUM_DATA_BITMASK;
    number_u >>= VARNUM_DATA_BITS_LEN;
    byte |= number_u > 0 ? VARNUM_SIGNAL_BIT : 0;
    err = CMC_ERR_ABLE(cmc_u8_pack(buff, byte), return err)
    if (number_u == 0) {
      break;
    }
  }
  return CMC_ERR_NO;
}

cmc_varint cmc_varint_unpack(cmc_span *buff, cmc_err *err) {
  cmc_varint ret = 0;
  for (int i = 0; i < MAX_VARINT_SIZE_BYTES; ++i) {
    uint8_t byte = CMC_ERRP_ABLE(cmc_u8_unpack(buff, err), return 0)
    ret |= (byte & VARNUM_DATA_BITMASK) << i * VARNUM_DATA_BITS_LEN;
    if (!(byte & VARNUM_SIGNAL_BIT)) {
      break;
    }
  }
  return ret;
}

cmc_err cmc_varlong_pack(cmc_buff *buff, cmc_varlong number) {
  assert(buff);
  uint64_t number_u = number;

  cmc_err err = CMC_ERR_NO;
  for (int i = 0; i < MAX_VARLONG_SIZE_BYTES; ++i) {
    uint8_t byte = number_u & VARNUM_DATA_BITMASK;
    number_u >>= VARNUM_DATA_BITS_LEN;
    byte |= number_u > 0 ? VARNUM_SIGNAL_BIT : 0;
    err = CMC_ERR_ABLE(cmc_u8_pack(buff, byte), return err;);
    if (number_u == 0) {
      break;
    }
  }
  return CMC_ERR_NO;
}

cmc_varlong cmc_varlong_unpack(cmc_span *buff, cmc_err *err) {
  cmc_varlong ret = 0;
  for (int i = 0; i < MAX_VARLONG_SIZE_BYTES; ++i) {
    uint8_t byte = CMC_ERRP_ABLE(cmc_u8_unpack(buff, err), return 0;);
    ret |= ((cmc_varlong)(byte & VARNUM_DATA_BITMASK))
           << (i * VARNUM_DATA_BITS_LEN);
    if (!(byte & VARNUM_SIGNAL_BIT)) {
      break;
    }
  }
  return ret;
}
