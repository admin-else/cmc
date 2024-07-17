#include "cmc/buff.h"
#include <endian.h>

#define C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(name, type, bits)                   \
  type cmc_unpack_##name(cmc_buff_unpacking *buff, cmc_err *err) {             \
    void *p = cmc_unpack(buff, sizeof(type), err);                             \
    if (*err != CMC_ERR_NO) {                                                  \
      return 0;                                                                \
    }                                                                          \
    return le##bits##toh(*(type *)p);                                          \
  }                                                                            \
                                                                               \
  cmc_err cmc_pack_##name(cmc_buff_packing *buff, type data) {                 \
    return htole##bits(cmc_pack(buff, &data, sizeof(type)));                                                         \
  }

#define C_TYPES_PACK_UNPACK_FACTORY(name, type)                                \
  type cmc_unpack_##name(cmc_buff_unpacking *buff, cmc_err *err) {             \
    void *p = cmc_unpack(buff, sizeof(type), err);                             \
    if (*err != CMC_ERR_NO) {                                                  \
      return 0;                                                                \
    }                                                                          \
    return *(type *)p;                                                         \
  }                                                                            \
                                                                               \
  cmc_err cmc_pack_##name(cmc_buff_packing *buff, type data) {                 \
    return cmc_pack(buff, &data, sizeof(type));                                \
  }

C_TYPES_PACK_UNPACK_FACTORY(i8, int8_t)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(i16, int16_t, 16)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(i32, int32_t, 32)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(i64, int64_t, 64)
C_TYPES_PACK_UNPACK_FACTORY(u8, uint8_t)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(u16, uint16_t, 16)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(u32, uint32_t, 32)
C_TYPES_PACK_UNPACK_FACTORY_ENDIAN(u64, uint64_t, 64)
C_TYPES_PACK_UNPACK_FACTORY(f32, float)
C_TYPES_PACK_UNPACK_FACTORY(f64, double)

// This has to be custom becouse the len may not be one byte https://en.cppreference.com/w/cpp/language/types
bool cmc_unpack_bool(cmc_buff_unpacking *buff, cmc_err *err) {
  void *p = cmc_unpack(buff, 1, err);
  if (*err != CMC_ERR_NO) {
    return 0;
  }
  return *(bool *)p;
}

cmc_err cmc_pack_bool(cmc_buff_packing *buff, bool data) {
  return cmc_pack(buff, &data, 1);
}

#undef C_TYPES_PACK_UNPACK_FACTORY
#undef C_TYPES_PACK_UNPACK_FACTORY_ENDIAN
