
#include "cmc/buff.h"
#include "cmc/types/c_types.h"
#include "cmc/types/varnums.h"
#include <stdio.h>

#define TEST(code)                                                             \
  err = code;                                                                  \
  if (err != CMC_ERR_NO) {                                                     \
    printf("I got a %s on line %d.\n", cmc_err_as_str(err), __LINE__);         \
    return 1;                                                                  \
  }

#define TYPES_PACK_UNPACK_FACTORY(name, type)                                  \
  cmc_err test_##name(type val) {                                              \
    cmc_buff buff1 = {0};                                                      \
    cmc_err err = cmc_##name##_pack(&buff1, val);                              \
    if (err != CMC_ERR_NO) {                                                   \
      return err;                                                              \
    }                                                                          \
    cmc_span buff2 = {                                                         \
        .data = buff1.data, .length = buff1.length, .position = 0};            \
    type val2 = cmc_##name##_unpack(&buff2, &err);                             \
    if (err != CMC_ERR_NO) {                                                   \
      return err;                                                              \
    }                                                                          \
    cmc_buff_free(&buff1);                                                     \
    if (val != val2) {                                                         \
      return CMC_ERR_NO_MATCH;                                                 \
    }                                                                          \
    return CMC_ERR_NO;                                                         \
  }

TYPES_PACK_UNPACK_FACTORY(i8, int8_t)
TYPES_PACK_UNPACK_FACTORY(i16, int16_t)
TYPES_PACK_UNPACK_FACTORY(i32, int32_t)
TYPES_PACK_UNPACK_FACTORY(i64, int64_t)
TYPES_PACK_UNPACK_FACTORY(u8, uint8_t)
TYPES_PACK_UNPACK_FACTORY(u16, uint16_t)
TYPES_PACK_UNPACK_FACTORY(u32, uint32_t)
TYPES_PACK_UNPACK_FACTORY(u64, uint64_t)
TYPES_PACK_UNPACK_FACTORY(f32, float)
TYPES_PACK_UNPACK_FACTORY(f64, double)
TYPES_PACK_UNPACK_FACTORY(bool, bool)
TYPES_PACK_UNPACK_FACTORY(varint, cmc_varint)
TYPES_PACK_UNPACK_FACTORY(varlong, cmc_varlong)

#undef TYPES_PACK_UNPACK_FACTORY

int main(void) { // NOLINT(readability-function-cognitive-complexity)
                 // this is justifyable
  cmc_err err = CMC_ERR_NO;
  TEST(test_bool(true));
  TEST(test_bool(false));

  TEST(test_i8(INT8_MIN));
  TEST(test_i8(INT8_MAX));
  TEST(test_i8(0));
  TEST(test_i8(-1));

  TEST(test_i16(INT16_MIN));
  TEST(test_i16(INT16_MAX));
  TEST(test_i16(0));
  TEST(test_i16(-1));

  TEST(test_i32(INT32_MIN));
  TEST(test_i32(INT32_MAX));
  TEST(test_i32(0));
  TEST(test_i32(-1));

  TEST(test_i64(INT64_MIN));
  TEST(test_i64(INT64_MAX));
  TEST(test_i64(0));
  TEST(test_i64(-1));

  TEST(test_u8(UINT8_MAX));
  TEST(test_u8(0));

  TEST(test_u16(UINT16_MAX));
  TEST(test_u16(0));

  TEST(test_u32(UINT32_MAX));
  TEST(test_u32(0));

  TEST(test_u64(UINT64_MAX));
  TEST(test_u64(0));

  TEST(test_varint(INT32_MIN));
  TEST(test_varint(INT32_MAX));
  TEST(test_varint(0));
  TEST(test_varint(-1));

  TEST(test_varlong(0));
  TEST(test_varlong(-1));
  TEST(test_varlong(INT64_MAX));
  TEST(test_varlong(INT64_MIN));

#undef TEST
  return 0;
}
