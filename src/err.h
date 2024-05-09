#pragma once

/*
ALL errors if they are not in the ctx object are in cmc_err_extra *err
to ensure these macros work
*/

#include <cmc/err.h>

#define CMC_ERR(err_type, action)                                              \
  do {                                                                         \
    *err =                                                                     \
        (cmc_err_extra){.file = __FILE__, .line = __LINE__, .err = err_type};  \
    action;                                                                    \
  } while (0)

#define CMC_ERRR(err_type)                                                     \
  do {                                                                         \
    *err =                                                                     \
        (cmc_err_extra){.file = __FILE__, .line = __LINE__, .err = err_type};  \
    return err_type;                                                           \
  } while (0)

#define CMC_ERRRB_ABLE(code)                                                   \
  code;                                                                        \
  if (buff->err.err)                                                           \
    return buff->err.err;

#define CMC_ERRB(err_type, action)                                             \
  do {                                                                         \
    buff->err =                                                                \
        (cmc_err_extra){.file = __FILE__, .line = __LINE__, .err = err_type};  \
    action;                                                                    \
  } while (0)

#define CMC_ERRB_ABLE(code, action)                                            \
  code;                                                                        \
  if (buff->err.err) {                                                         \
    action;                                                                    \
  }

#define CMC_ERRB_IF(condition, err_type, action)                               \
  if (condition) {                                                             \
    CMC_ERRB(err_type, action);                                                \
  }

#define CMC_ERRRB(err_type)                                                    \
  do {                                                                         \
    buff->err =                                                                \
        (cmc_err_extra){.file = __FILE__, .line = __LINE__, .err = err_type};  \
    return err_type;                                                           \
  } while (0)