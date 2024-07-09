#pragma once
#include <assert.h>

#define CMC_ERR_ENUM_VALUES_X                                                  \
  X(CMC_ERR_NO)                                                                \
  X(CMC_ERR_MALLOC_ZERO)                                                       \
  X(CMC_ERR_MALLOC_FAIL)

typedef enum {
#define X(ERR) ERR,
  CMC_ERR_ENUM_VALUES_X
#undef X
} cmc_err;

#define X(ERR) +1 // NOLINT(bugprone-macro-parentheses)
enum { cmc_err_enum_size = 0 CMC_ERR_ENUM_VALUES_X };
#undef X

extern const char *const cmc_err_strings[cmc_err_enum_size];

inline const char *cmc_err_as_str(cmc_err err) {
  assert(0 <= err && err < (cmc_err)(cmc_err_enum_size));
  return cmc_err_strings[err];
}
