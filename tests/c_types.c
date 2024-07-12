
#include "cmc/types/c_types.h"
#include "cmc/buff.h"
#include <stdio.h>

#define TEST(code)                                                             \
  err = code;                                                                  \
  if (err != CMC_ERR_NO) {                                                     \
    printf("I got a %s on line %d.\n", cmc_err_as_str(err), __LINE__);         \
    return 1;                                                                  \
  }

#define C_TYPES_PACK_UNPACK_FACTORY(name, type)                                \
  cmc_err test_##name(type val) {                                              \
    cmc_buff_packing buff1 = {0};                                              \
    cmc_err err = cmc_pack_##name(&buff1, val);                                \
    if (err != CMC_ERR_NO) {                                                   \
      return err;                                                              \
    }                                                                          \
    cmc_buff_unpacking buff2 = {                                               \
        .data = buff1.data, .length = buff1.length, .position = 0};            \
    type val2 = cmc_unpack_##name(&buff2, &err);                               \
    if (err != CMC_ERR_NO) {                                                   \
      return err;                                                              \
    }                                                                          \
    cmc_buff_free(&buff1);                                                     \
    if (val != val2) {                                                         \
      return CMC_ERR_NO_MATCH;                                                 \
    }                                                                          \
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

#undef C_TYPES_PACK_UNPACK_FACTORY

cmc_err test_bool(bool val) {
  cmc_buff_packing buff1 = {0};
  cmc_err err = cmc_pack_bool(&buff1, val);
  if (err != CMC_ERR_NO) {
    return err;
  }
  cmc_buff_unpacking buff2 = {
      .data = buff1.data, .length = buff1.length, .position = 0};
  bool val2 = cmc_unpack_bool(&buff2, &err);
  if (err != CMC_ERR_NO) {
    return err;
  }
  cmc_buff_free(&buff1);
  if (val != val2) {
    return CMC_ERR_NO_MATCH;
  }
  return CMC_ERR_NO;
}
int main() {
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

#undef TEST

  return 0;
}
