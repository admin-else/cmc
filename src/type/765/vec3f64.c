









#include<cmc/err_macros.h>
#include<cmc/type/c_types.h>
#include<cmc/err.h>
#include<cmc/type/765/vec3f64.h>

cmc_765_vec3f64 cmc_765_vec3f64_unpack(cmc_span *buff, cmc_err *err) {
    
    cmc_765_vec3f64 data = (cmc_765_vec3f64){0};
    
data.x = CMC_ERRP_ABLE(cmc_f64_unpack(buff, err), return (cmc_765_vec3f64){0};)

data.y = CMC_ERRP_ABLE(cmc_f64_unpack(buff, err), return (cmc_765_vec3f64){0};)

data.z = CMC_ERRP_ABLE(cmc_f64_unpack(buff, err), return (cmc_765_vec3f64){0};)

    return data;
}

cmc_err cmc_765_vec3f64_pack(cmc_buff *buff, cmc_765_vec3f64 data) {
    
    cmc_err err = CMC_ERR_NO;
    
err = CMC_ERR_ABLE(cmc_f32_pack(buff, data.x), return CMC_ERR_NO;)

err = CMC_ERR_ABLE(cmc_f32_pack(buff, data.y), return CMC_ERR_NO;)

err = CMC_ERR_ABLE(cmc_f32_pack(buff, data.z), return CMC_ERR_NO;)

    return err;
}