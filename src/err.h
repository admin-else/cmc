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
  ERR_INVALID_PACKET_ID_WHILE_LOGGING_IN,
  ERR_MALLOC_ZERO,
  ERR_INVALID_ARGUMENTS,
  ERR_BUFFER_UNDERUN,
  ERR_BUFFER_OVERFLOW,
  ERR_STRING_LENGHT,
  ERR_INVALID_STRING
};
struct cmc_err {
  char *file;
  int line;
  enum cmc_err_type type;
};

extern struct cmc_err cmc_err;

#define ERR(err)                                                               \
  do {                                                                         \
    cmc_err =                                                                  \
        (struct cmc_err){.file = __FILE__, .line = __LINE__, .type = err};     \
    ERR_ACTION                                                                 \
  } while (0)
#define ERR_CHECK                                                              \
  if (cmc_err.type) {                                                          \
    ERR_ACTION                                                                 \
  }
#define ERR_ABLE(code)                                                         \
  code;                                                                        \
  ERR_CHECK
#define ERR_IF(conditon, err)                                                  \
  do {                                                                         \
    if (conditon) {                                                            \
      ERR(err);                                                                \
    }                                                                          \
  } while (0)
#define ERR_IF_NOT(val, err) ERR_IF(!(val), err)
#define ERR_IF_VAL_TO_CONDITION(val, conditon, err) ERR_IF(val conditon, err)
#define ERR_IF_LESS_THAN_ZERO(val, err) ERR_IF_VAL_TO_CONDITION(val, < 0, err)
#define ERR_IF_NOT_ZERO(val, err) ERR_IF_VAL_TO_CONDITION(val, != 0, err)
#define ERR_IF_LESS_OR_EQ_TO_ZERO(val, err)                                    \
  ERR_IF_VAL_TO_CONDITION(val, <= 0, err)
