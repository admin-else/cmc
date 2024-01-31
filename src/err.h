#pragma once

#define ERR_IF_NOT(val, msg)                                                   \
  do {                                                                         \
    if (!(val)) {                                                              \
      *errmsg = msg;                                                           \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ERR_ABLE(code)                                                         \
  code;                                                                        \
  do {                                                                         \
    if (*errmsg != NULL)                                                       \
      return;                                                                  \
  } while (0)
