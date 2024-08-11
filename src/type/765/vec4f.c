
    

    

    

    



#include<cmc/type/c_types.h>
#include<cmc/err.h>
#include<cmc/type/765/vec4f.h>

cmc_err cmc_765_vec4f_pack(cmc_buff *buff, cmc_765_vec4f data) {
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

err = cmc_f32_pack(buff, data.w);
if(err != CMC_ERR_NO) {
    return err;
}

  return err;
}

cmc_765_vec4f cmc_765_vec4f_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_vec4f data;
  
  return data;
}