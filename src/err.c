#include "err.h"

const char *cmc_err_as_str(cmc_err_auto err) {
#define ERRID2STR_HELPER(ERR)                                                  \
  case ERR:                                                                    \
    return #ERR;
#if CMC_ERR_EXTRA
  switch (err.err_type) {
#else
  switch (err) {
#endif
    // CGSS: error_helper
    ERRID2STR_HELPER(CMC_ERR_NO)
    ERRID2STR_HELPER(CMC_ERR_MEM)
    ERRID2STR_HELPER(CMC_ERR_CONNETING)
    ERRID2STR_HELPER(CMC_ERR_SOCKET)
    ERRID2STR_HELPER(CMC_ERR_CLOSING)
    ERRID2STR_HELPER(CMC_ERR_RECV)
    ERRID2STR_HELPER(CMC_ERR_INVALID_PACKET_LEN)
    ERRID2STR_HELPER(CMC_ERR_ZLIB_INIT)
    ERRID2STR_HELPER(CMC_ERR_ZLIB_INFLATE)
    ERRID2STR_HELPER(CMC_ERR_SENDER_LYING)
    ERRID2STR_HELPER(CMC_ERR_ZLIB_COMPRESS)
    ERRID2STR_HELPER(CMC_ERR_SENDING)
    ERRID2STR_HELPER(CMC_ERR_KICKED_WHILE_LOGIN)
    ERRID2STR_HELPER(CMC_ERR_SERVER_ONLINE_MODE)
    ERRID2STR_HELPER(CMC_ERR_UNKOWN_PACKET)
    ERRID2STR_HELPER(CMC_ERR_MALLOC_ZERO)
    ERRID2STR_HELPER(CMC_ERR_INVALID_ARGUMENTS)
    ERRID2STR_HELPER(CMC_ERR_BUFFER_UNDERUN)
    ERRID2STR_HELPER(CMC_ERR_BUFFER_OVERFLOW)
    ERRID2STR_HELPER(CMC_ERR_STRING_LENGHT)
    ERRID2STR_HELPER(CMC_ERR_INVALID_STRING)
    ERRID2STR_HELPER(CMC_ERR_INVALID_LENGHT)
    ERRID2STR_HELPER(CMC_ERR_INVALID_NBT_TAG_TYPE)
    ERRID2STR_HELPER(CMC_ERR_NOT_IMPLEMENTED_YET)
    ERRID2STR_HELPER(CMC_ERR_ASSERT)
    ERRID2STR_HELPER(CMC_ERR_UNSUPPORTED_PROTOCOL_VERSION)
    ERRID2STR_HELPER(CMC_ERR_UNEXPECTED_PACKET)
// CGSE: error_helper
#undef ERRID2STR_HELPER
  default:
    return "invalid error id";
  }
}