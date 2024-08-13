




#include<cmc/type/pstring.h>
#include<string.h>
#include<cmc/err_macros.h>
#include<cmc/err.h>
#include<cmc/type/765/string.h>

cmc_err cmc_765_string_pack(cmc_buff *buff, cmc_765_string data) {
  cmc_err err = CMC_ERR_NO;
  
  CMC_ERR_ABLE(cmc_pstring_pack(buff, data, strlen(data)), );
  return err;
}

cmc_765_string cmc_765_string_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_string data;
  {
  size_t len = cmc__unpack(buff, err);
  if(*err != CMC_ERR_NO) {
    return ({cmc_765_string}){0};
  }
   = cmc_unpack_pstring(buff, err, len);
}
  return data;
}