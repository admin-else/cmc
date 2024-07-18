#include "cmc/types/varnums.h"
#include "cmc/buff.h"
#include "cmc/err.h"
#include "cmc/types/c_types.h"

#define VARNUM_DATA_BITMASK (uint8_t)(0x7F)
#define VARNUM_SIGNAL_BIT (uint8_t)(0x80)
#define MAX_VARINT_SIZE_BYTES (int)5
#define MAX_VARLONG_SIZE_BYTES (int)10
#define VARNUM_DATA_BITS_LEN (int)7

cmc_err cmc_varint_pack(cmc_buff_packing *buff, cmc_varint number) {
  assert(buff);
  uint32_t number_u = *(uint32_t *)&number;
  cmc_err err = CMC_ERR_NO;
  for (int i = 0; i < MAX_VARINT_SIZE_BYTES; ++i) {
    uint8_t b = number_u & VARNUM_DATA_BITMASK;
    number_u >>= VARNUM_DATA_BITS_LEN;
    b |= number_u > 0 ? VARNUM_SIGNAL_BIT : 0;
    err = cmc_u8_pack(buff, b);
    if (err != CMC_ERR_NO) {
      return err;
    }
    if (number_u == 0)
      break;
  }
  return CMC_ERR_NO;
}

cmc_varint cmc_varint_unpack(cmc_buff_unpacking *buff, cmc_err *err) {
  cmc_varint ret = 0;
  for (int i = 0; i < MAX_VARINT_SIZE_BYTES; ++i) {
    uint8_t b = cmc_u8_unpack(buff, err);
    if (*err != CMC_ERR_NO) {
      return 0;
    }
    ret |= (b & VARNUM_DATA_BITMASK) << i * VARNUM_DATA_BITS_LEN;
    if (!(b & VARNUM_SIGNAL_BIT))
      break;
  }
  return ret;
}

cmc_err cmc_varlong_pack(cmc_buff_packing *buff, cmc_varlong number) {
  assert(buff);
  uint64_t number_u = *(uint64_t *)&number;
  cmc_err err = CMC_ERR_NO;
  for (int i = 0; i < MAX_VARLONG_SIZE_BYTES; ++i) {
    uint8_t b = number_u & VARNUM_DATA_BITMASK;
    number_u >>= VARNUM_DATA_BITS_LEN;
    b |= number_u > 0 ? VARNUM_SIGNAL_BIT : 0;
    err = cmc_u8_pack(buff, b);
    if (err != CMC_ERR_NO) {
      return err;
    }
    if (number_u == 0)
      break;
  }
  return CMC_ERR_NO;
}

cmc_varlong cmc_varlong_unpack(cmc_buff_unpacking *buff, cmc_err *err) {
  cmc_varlong ret = 0;
  for (int i = 0; i < MAX_VARLONG_SIZE_BYTES; ++i) {
    uint8_t b = cmc_u8_unpack(buff, err);
    if (*err != CMC_ERR_NO) {
      return 0;
    }
    ret |= (b & VARNUM_DATA_BITMASK) << i * VARNUM_DATA_BITS_LEN;
    if (!(b & VARNUM_SIGNAL_BIT))
      break;
  }
  return ret;
}
