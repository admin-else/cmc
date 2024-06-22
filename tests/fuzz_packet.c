

#include <stddef.h>
#include <stdint.h>
#include <cmc/packets.h>

#define packet_to_fuzz S2C_play_entity_properties

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) { 
    cmc_buff *buff = cmc_buff_init(47);
    cmc_buff_pack(buff, data, size);
    return 0; 
}
