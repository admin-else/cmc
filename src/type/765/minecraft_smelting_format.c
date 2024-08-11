
    

    

    

    

    

    



#include<cmc/type/varnums.h>
#include<cmc/type/c_types.h>
#include<cmc/err.h>
#include<cmc/type/765/minecraft_smelting_format.h>

cmc_err cmc_765_minecraft_smelting_format_pack(cmc_buff *buff, cmc_765_minecraft_smelting_format data) {
  cmc_err err = CMC_ERR_NO;
  
  
 = cmc_765_string_pack(buff, err);



 = cmc_765_ingredient_pack(buff, err);

 = cmc_765_slot_pack(buff, err);

err = cmc_f32_pack(buff, data.experience);
if(err != CMC_ERR_NO) {
    return err;
}



  return err;
}

cmc_765_minecraft_smelting_format cmc_765_minecraft_smelting_format_unpack(cmc_span *span, cmc_err *err) {
  
  cmc_765_minecraft_smelting_format data;
  
  return data;
}