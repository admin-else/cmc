
    


    


    




#include<cmc/type/c_types.h>
#include<cmc/err_macros.h>
#include<cmc/err.h>
#include<cmc/type/765/vec3f.h>

cmc_err cmc_765_vec3f_pack(cmc_buff *buff, cmc_765_vec3f data) {
  cmc_err err = CMC_ERR_NO;
  
  
err = cmc_f32_pack(buff, data.x);
if(err != CMC_ERR_NO) {
    return err;
}

err = cmc_f32_pack(buff, data.y);
if(err != CMC_ERR_NO) {
    return err;
}

err = cmc_f32_pack(buff, data.z);
if(err != CMC_ERR_NO) {
    return err;
}

  return err;
}

cmc_765_vec3f cmc_765_vec3f_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_vec3f data;
  
data.x = CMC_ERR_ABLE(cmc_f32_unpack(span, err), return ({cmc_765_vec3f}){0};);

data.y = CMC_ERR_ABLE(cmc_f32_unpack(span, err), return ({cmc_765_vec3f}){0};);

data.z = CMC_ERR_ABLE(cmc_f32_unpack(span, err), return ({cmc_765_vec3f}){0};);

  return data;
}