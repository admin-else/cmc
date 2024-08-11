


#include<cmc/type/pstring.h>
#include<cmc/err.h>
#include<cmc/type/765/string.h>

cmc_err cmc_765_string_pack(cmc_buff *buff, cmc_765_string data) {
  cmc_err err = CMC_ERR_NO;
  
  size_t len = cmc__unpack(buff, err);
if(*err != CMC_ERR_NO) {
  return err;
}
 = cmc_pstring_unpack(buff, err);
if(*err != CMC_ERR_NO) {
  return err;
}
  return err;
}

cmc_765_string cmc_765_string_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_string data;
  {
  size_t len = cmc__unpack(buff, err);
  if(*err != CMC_ERR_NO) {
    return ({cmc_765_string}){0};
  }
  data = cmc_unpack_pstring(buff, err, len);
}
  return data;
}