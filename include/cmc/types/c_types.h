#include "cmc/buff.h"
#include <stdint.h>

#define C_TYPES_PACK_UNPACK_FACTORY_H(name, type)                              \
  type cmc_##name##_unpack(cmc_buff_unpacking *buff, cmc_err *err);            \
  cmc_err cmc_##name##_pack(cmc_buff_packing *buff, type data);

C_TYPES_PACK_UNPACK_FACTORY_H(i8, int8_t)
C_TYPES_PACK_UNPACK_FACTORY_H(i16, int16_t)
C_TYPES_PACK_UNPACK_FACTORY_H(i32, int32_t)
C_TYPES_PACK_UNPACK_FACTORY_H(i64, int64_t)
C_TYPES_PACK_UNPACK_FACTORY_H(u8, uint8_t)
C_TYPES_PACK_UNPACK_FACTORY_H(u16, uint16_t)
C_TYPES_PACK_UNPACK_FACTORY_H(u32, uint32_t)
C_TYPES_PACK_UNPACK_FACTORY_H(u64, uint64_t)
C_TYPES_PACK_UNPACK_FACTORY_H(f32, float)
C_TYPES_PACK_UNPACK_FACTORY_H(f64, double)
C_TYPES_PACK_UNPACK_FACTORY_H(bool, bool)

#undef C_TYPES_PACK_UNPACK_FACTORY_H
