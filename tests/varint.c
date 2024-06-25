#include <cmc/buff.h>
#include <stdio.h>
#include <stdlib.h>

enum { test_amount = 1000000 };

#define TRY_CATCH(expr, action)                                                \
  if (expr != CMC_ERR_NO) {                                                    \
    action;                                                                    \
  }

int main() {
  cmc_buff *buff = NULL;
  for (int32_t i; i < test_amount; ++i) {
    buff = cmc_buff_init(47);
    int32_t num = rand() % INT32_MAX;
    cmc_buff_pack_varint(buff, num);
    TRY_CATCH(buff->err.err, goto err_print;);
    int32_t decoded_val = cmc_buff_unpack_varint(buff);
    TRY_CATCH(buff->err.err, goto err_print;);
    if (decoded_val != num) {
      printf("cmc's decoded(%i) value wasnt the one encoded(%i).\n", decoded_val, num);
      goto err_free_buff;
    }
    cmc_buff_free(buff);
  }
  return 0;
err_print:
  printf("cmc errd at %s:%d with error %s\n", buff->err.file, buff->err.line,
         cmc_err_as_str(buff->err.err));
err_free_buff:
  cmc_buff_free(buff);
  return 1;
}
