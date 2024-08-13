
    

    

    

    

    


    

#include<cmc/type/varnums.h>
#include<cmc/type/c_types.h>
#include<cmc/err_macros.h>

typedef struct {
  
    cmc_765_string group;
  
    cmc_varint category;
  
    cmc_765_ingredient ingredient;
  
    cmc_765_slot result;
  
    float experience;
  
    cmc_varint cookTime;
  
}  cmc_765_minecraft_smelting_format;