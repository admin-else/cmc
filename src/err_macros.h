#pragma once
#include <cmc/err.h>

/*
ALL errors if they are not in the ctx object are in cmc_err_extra *err
to ensure these macros work

CMC_ERR???

A(bstaract): parent of all other types you can also use it in weird situations
R(eturns): returns error in the function instead of reqiring action
C(onn): writes err into conn instead of err
B(uff): like conn but into buffer
*/

#define CMC_ERRA(err_type, action, err_var)                                    \
  do {                                                                         \
    err_var =                                                                  \
        (cmc_err_extra){.file = __FILE__, .line = __LINE__, .err = err_type};  \
    action;                                                                    \
  } while (0)

#define CMC_ERRA_IF(condition, err_var, err_type, action)                      \
  if (condition) {                                                             \
    CMC_ERRA(err_type, action, err_var);                                       \
  }

#define CMC_ERR(err_type, action) CMC_ERRA(err_type, action, *err)
#define CMC_ERRR(err_type) CMC_ERR(err_type, return err_type)

#define CMC_ERRB(err_type, action) CMC_ERRA(err_type, action, *buff->err)

#define CMC_ERRB_IF(condition, err_type, action)                               \
  CMC_ERRA_IF(condition, *buff->err, err_type, action)

#define CMC_ERRRB(err_type) CMC_ERRA(err_type, return err_type;, *buff->err)

#define CMC_ERRRC_IF(condition, err_type)                                      \
  CMC_ERRA_IF(condition, conn->err, err_type, return err_type)

#define CMC_ERRC_IF(condition, err_type, action)                               \
  CMC_ERRA_IF(condition, conn->err, err_type, action)

#define CMC_ERRC(code, action) CMC_ERRA(code, conn->err.err, action)
