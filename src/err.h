#pragma once

#define CALL(code)            \
  do                          \
  {                           \
    code if (*errmsg != NULL) \
    {                         \
      goto err_h;             \
    }                         \
  } while (0)