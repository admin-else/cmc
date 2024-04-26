#pragma once

enum cmc_err_type {
  ERR_NO,
  ERR_MEM,
  ERR_CONNETING,
  ERR_SOCKET,
  ERR_CLOSING,
  ERR_RECV,
  ERR_INVALID_PACKET_LEN,
  ERR_ZLIB_INIT,
  ERR_ZLIB_INFLATE,
  ERR_SENDER_LYING,
  ERR_ZLIB_COMPRESS,
  ERR_SENDING,
  ERR_KICKED_WHILE_LOGIN,
  ERR_SERVER_ONLINE_MODE,
  ERR_UNKOWN_PACKET,
  ERR_MALLOC_ZERO,
  ERR_INVALID_ARGUMENTS,
  ERR_BUFFER_UNDERUN,
  ERR_BUFFER_OVERFLOW,
  ERR_STRING_LENGHT,
  ERR_INVALID_STRING,
  ERR_INVALID_LENGHT,
  ERR_INVALID_NBT_TAG_TYPE,
  ERR_NOT_IMPLEMENTED_YET,
  ERR_ASSERT,
  ERR_UNSUPPORTED_PROTOCOL_VERSION,
  ERR_UNEXPECTED_PACKET
};

#define CMC_ERR_EXTRA 1

#if CMC_ERR_EXTRA
struct cmc_err_extra {
  enum cmc_err_type err_type;
  const char *file;
  int line;
};

typedef struct cmc_err_extra cmc_err_auto;

#define ERR(err, action)                                                       \
  do {                                                                         \
    cmc_err = (struct cmc_err_extra){                                          \
        .file = __FILE__, .line = __LINE__, .err_type = err};                  \
    action                                                                     \
  } while (0)
#define ERR_CHECK(action)                                                      \
  if (cmc_err.err_type) {                                                      \
    action                                                                     \
  }
#define ERR_ABLE(code, action)                                                 \
  code;                                                                        \
  ERR_CHECK(action)


#else
#define ERR(err, action)                                                       \
  do {                                                                         \
    cmc_err = err;                                                             \
    action                                                                     \
  } while (0)
#define ERR_CHECK(action)                                                      \
  if (cmc_err) {                                                               \
    action                                                                     \
  }
#define ERR_ABLE(code, action)                                                 \
  code;                                                                        \
  ERR_CHECK(action)


typedef enum cmc_err_type cmc_err_auto;

#endif

#define ERR_IF(conditon, err, action)                                          \
  {                                                                            \
    if (conditon) {                                                            \
      ERR(err, action);                                                        \
    }                                                                          \
  }
#define ERR_IF_NOT(val, err, action) ERR_IF(!(val), err, action)
#define ERR_IF_VAL_TO_CONDITION(val, conditon, err, action)                    \
  ERR_IF(val conditon, err, action)
#define ERR_IF_LESS_THAN_ZERO(val, err, action)                                \
  ERR_IF_VAL_TO_CONDITION(val, < 0, err, action)
#define ERR_IF_NOT_ZERO(val, err, action)                                      \
  ERR_IF_VAL_TO_CONDITION(val, != 0, err, action)
#define ERR_IF_LESS_OR_EQ_TO_ZERO(val, err, action)                            \
  ERR_IF_VAL_TO_CONDITION(val, <= 0, err, action)

extern cmc_err_auto cmc_err;

const char *cmc_err_as_str(cmc_err_auto err);