#include <cmc/err.h>
#include <cmc/packets.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define PACKET_TO_FUZZ        cmc_packet_S2C_play_entity_properties_47
#define PACKET_TO_FUZZ_UNPACK cmc_packet_S2C_play_entity_properties_47_unpack
#define PACKET_TO_FUZZ_PACK   cmc_packet_S2C_play_entity_properties_47_pack
#define PACKET_TO_FUZZ_FREE   cmc_packet_S2C_play_entity_properties_47_free
#define PACKET_TO_FUZZ_VERS   47

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  cmc_err_extra err = {0};
  cmc_buff *buff = cmc_buff_init(PACKET_TO_FUZZ_VERS, &err);
  CMC_ERRB_ABLE(cmc_buff_pack(buff, data, size), goto err_pre_packet;);
  CMC_ERRB_ABLE(cmc_buff_unpack_varint(buff), goto err_pre_packet;);
  PACKET_TO_FUZZ packet =
      CMC_ERRB_ABLE(PACKET_TO_FUZZ_UNPACK(buff), goto err_pre_packet;);
  cmc_buff *tmp = cmc_buff_init(PACKET_TO_FUZZ_VERS, &err);
  CMC_ERRA_ABLE(PACKET_TO_FUZZ_PACK(tmp, &packet), tmp->err->err,
                goto err_after_tmp);
  if (!cmc_buff_compare(buff, tmp)) {
    puts("not equal");
    goto err_after_tmp;
  }
  cmc_buff_free(tmp);
  cmc_buff_free(buff);
  PACKET_TO_FUZZ_FREE(&packet);
  puts("all good (:");
  return 0;
err_after_tmp:
  cmc_buff_free(tmp);
err_post_packet:
  PACKET_TO_FUZZ_FREE(&packet);
err_pre_packet:
  cmc_buff_free(buff);

  if(err.err) {
    printf("Error %s at %s:%i\n", cmc_err_as_str(err.err), err.file, err.line);
  }
  return 1;
}
