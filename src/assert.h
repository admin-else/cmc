#pragma once
#include "err.h"

#define assert(con, action)                                                    \
  if (!(con))                                                                  \
    ERR(ERR_ASSERT, action);
