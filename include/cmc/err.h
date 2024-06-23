#pragma once

#include <assert.h>

#define CMC_ERR_ENUM_VALUES_X                                                  \
  X(CMC_ERR_NO)                                                                \
  X(CMC_ERR_MEM)                                                               \
  X(CMC_ERR_CONNETING)                                                         \
  X(CMC_ERR_SOCKET)                                                            \
  X(CMC_ERR_CLOSING)                                                           \
  X(CMC_ERR_RECV)                                                              \
  X(CMC_ERR_INVALID_PACKET_LENGTH)                                             \
  X(CMC_ERR_ZLIB_INIT)                                                         \
  X(CMC_ERR_ZLIB_INFLATE)                                                      \
  X(CMC_ERR_SENDER_LYING)                                                      \
  X(CMC_ERR_ZLIB_COMPRESS)                                                     \
  X(CMC_ERR_SENDING)                                                           \
  X(CMC_ERR_KICKED_WHILE_LOGIN)                                                \
  X(CMC_ERR_SERVER_ONLINE_MODE)                                                \
  X(CMC_ERR_UNKOWN_PACKET)                                                     \
  X(CMC_ERR_MALLOC_ZERO)                                                       \
  X(CMC_ERR_INVALID_ARGUMENTS)                                                 \
  X(CMC_ERR_BUFF_UNDERFLOW)                                                    \
  X(CMC_ERR_BUFF_OVERFLOW)                                                     \
  X(CMC_ERR_STRING_LENGTH)                                                     \
  X(CMC_ERR_INVALID_STRING)                                                    \
  X(CMC_ERR_INVALID_LENGTH)                                                    \
  X(CMC_ERR_INVALID_NBT_TAG_TYPE)                                              \
  X(CMC_ERR_NOT_IMPLEMENTED_YET)                                               \
  X(CMC_ERR_ASSERT)                                                            \
  X(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION)                                      \
  X(CMC_ERR_UNEXPECTED_PACKET)                                                 \
  X(CMC_ERR_REALLOC_ZERO)                                                      \
  X(CMC_ERR_NEGATIVE_STRING_LENGTH)                                            \
  X(CMC_ERR_NBT_UNEXPECTED_TAG_END)                                            \
  X(CMC_ERR_NBT_NEGATIVE_LIST_LEN)                                             \
  X(CMC_ERR_NBT_BAD_LIST)                                                      \
  X(CMC_ERR_NEGATIVE_BUFF_LEN)

typedef enum {
#define X(ERR) ERR,
  CMC_ERR_ENUM_VALUES_X
#undef X
} cmc_err;

#define X(ERR) +1 // NOLINT(bugprone-macro-parentheses)
enum { cmc_err_enum_size = 0 CMC_ERR_ENUM_VALUES_X };
#undef X

extern const char *const cmc_err_strings[cmc_err_enum_size];

typedef struct {
  cmc_err err;
  const char *file;
  int line;
} cmc_err_extra;

inline const char *cmc_err_as_str(cmc_err err) {
  assert(0 <= err && err < (cmc_err)(cmc_err_enum_size));
  return cmc_err_strings[err];
}

#define CMC_ERRA_ABLE(code, err_var, action)                                   \
  code;                                                                        \
  if (err_var) {                                                               \
    action;                                                                    \
  }

#define CMC_ERRRB_ABLE(code)                                                   \
  CMC_ERRA_ABLE(code, buff->err.err, return buff->err.err;)
#define CMC_ERRB_ABLE(code, action) CMC_ERRA_ABLE(code, buff->err.err, action)
#define CMC_ERRC_ABLE(code, action) CMC_ERRA_ABLE(code, conn->err.err, action)
#define CMC_ERR_ABLE(code, action) CMC_ERRA_ABLE(code, err->err, action)
