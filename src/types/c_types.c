#include "cmc/buff.h"

#define C_TYPES_PACK_UNPACK_FACTORY(name, type)                                \
  type cmc_unpack_##name(cmc_buff_unpacking *buff, cmc_err *err) {             \
    void *p = cmc_buff_unpack(buff, sizeof(type), err);                        \
    if (*err != CMC_ERR_NO) {                                                  \
      return 0;                                                                \
    }                                                                          \
    return *(type *)p;                                                         \
  }                                                                            \
                                                                               \
  cmc_err cmc_pack_##name(cmc_buff_packing *buff, type data) {                 \
    cmc_err err = CMC_ERR_NO;                                                  \
    cmc_buff_pack(buff, &data, sizeof(type));                                  \
    return CMC_ERR_NO;                                                         \
  }

C_TYPES_PACK_UNPACK_FACTORY(i8, int8_t)
C_TYPES_PACK_UNPACK_FACTORY(i16, int16_t)
C_TYPES_PACK_UNPACK_FACTORY(i32, int32_t)
C_TYPES_PACK_UNPACK_FACTORY(i64, int64_t)
C_TYPES_PACK_UNPACK_FACTORY(u8, uint8_t)
C_TYPES_PACK_UNPACK_FACTORY(u16, uint16_t)
C_TYPES_PACK_UNPACK_FACTORY(u32, uint32_t)
C_TYPES_PACK_UNPACK_FACTORY(u64, uint64_t)
C_TYPES_PACK_UNPACK_FACTORY(f32, float)
C_TYPES_PACK_UNPACK_FACTORY(f64, double)
C_TYPES_PACK_UNPACK_FACTORY(bool, bool)

#undef C_TYPES_PACK_UNPACK_FACTORY
