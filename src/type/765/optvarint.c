


#include<cmc/type/varnums.h>
#include<cmc/err.h>
#include<cmc/type/765/optvarint.h>

cmc_err cmc_765_optvarint_pack(cmc_buff *buff, cmc_765_optvarint data) {
  cmc_err err = CMC_ERR_NO;
  
  
  return err;
}

cmc_765_optvarint cmc_765_optvarint_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_optvarint data;
  data = cmc_varint_unpack(span, err);
  return data;
}