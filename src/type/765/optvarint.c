#include<cmc/type/varnums.h>
#include<cmc/err_macros.h>
#include<cmc/err.h>
#include<cmc/type/765/optvarint.h>

cmc_765_optvarint cmc_765_optvarint_unpack(cmc_span *buff, cmc_err *err) {
    
    cmc_765_optvarint data = (cmc_765_optvarint){0};
    data = CMC_ERRP_ABLE(cmc_varint_unpack(buff, err), return (cmc_765_optvarint){0};)
    return data;
}

cmc_err cmc_765_optvarint_pack(cmc_buff *buff, cmc_765_optvarint data) {
    
    cmc_err err = CMC_ERR_NO;
    err = CMC_ERR_ABLE(cmc_varint_pack(buff, data), return CMC_ERR_NO;)
    return err;
}
