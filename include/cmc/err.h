#pragma once

enum cmc_err_type {
  // CGSS: error_def
  CMC_ERR_NO,
  CMC_ERR_MEM,
  CMC_ERR_CONNETING,
  CMC_ERR_SOCKET,
  CMC_ERR_CLOSING,
  CMC_ERR_RECV,
  CMC_ERR_INVALID_PACKET_LEN,
  CMC_ERR_ZLIB_INIT,
  CMC_ERR_ZLIB_INFLATE,
  CMC_ERR_SENDER_LYING,
  CMC_ERR_ZLIB_COMPRESS,
  CMC_ERR_SENDING,
  CMC_ERR_KICKED_WHILE_LOGIN,
  CMC_ERR_SERVER_ONLINE_MODE,
  CMC_ERR_UNKOWN_PACKET,
  CMC_ERR_MALLOC_ZERO,
  CMC_ERR_INVALID_ARGUMENTS,
  CMC_ERR_BUFFER_UNDERUN,
  CMC_ERR_BUFFER_OVERFLOW,
  CMC_ERR_STRING_LENGHT,
  CMC_ERR_INVALID_STRING,
  CMC_ERR_INVALID_LENGHT,
  CMC_ERR_INVALID_NBT_TAG_TYPE,
  CMC_ERR_NOT_IMPLEMENTED_YET,
  CMC_ERR_ASSERT,
  CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION,
  CMC_ERR_UNEXPECTED_PACKET
  // CGSE: error_def
};

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