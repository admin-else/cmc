






#include<cmc/type/pstring.h>
#include<cmc/err_macros.h>
#include<string.h>
#include<cmc/type/varnums.h>
#include<cmc/err.h>
#include<cmc/type/765/string.h>

cmc_765_string cmc_765_string_unpack(cmc_span *buff, cmc_err *err) {
    
    cmc_765_string data = (cmc_765_string){0};
    {
cmc_varint len = 0;
 = CMC_ERRP_ABLE(cmc_varint_unpack(buff, err), return (cmc_765_string){0};)
data = CMC_ERRP_ABLE(cmc_pstring_unpack(buff, len, err), return (cmc_765_string){0};)
}
    return data;
}

cmc_err cmc_765_string_pack(cmc_buff *buff, cmc_765_string data) {
    
    cmc_err err = CMC_ERR_NO;
    err = CMC_ERR_ABLE(cmc_pstring_pack(buff,data,strlen(data)),return CMC_ERR_NO;)
    return err;
}