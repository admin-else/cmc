
#include "cmc/buff.h"
#include "cmc/types/c_types.h"
#include <stdio.h>

#define TEST(code)                                                             \
  err = code;                                                                  \
  if (err != CMC_ERR_NO) {                                                     \
    printf("I got a %s on line %d.\n", cmc_err_as_str(err), __LINE__);         \
    return 1;                                                                  \
  }

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
  return 0;
}
