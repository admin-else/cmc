#include <cmc/err.h>
#include <cmc/packets.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define PACKET_TO_FUZZ        cmc_packet_C2S_handshake_handshake_765
#define PACKET_TO_FUZZ_UNPACK cmc_packet_C2S_handshake_handshake_765_unpack
#define PACKET_TO_FUZZ_PACK   cmc_packet_C2S_handshake_handshake_765_pack
#define PACKET_TO_FUZZ_FREE   cmc_packet_C2S_handshake_handshake_765_free
#define PACKET_TO_FUZZ_VERS   47

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  cmc_buff *buff = cmc_buff_init(PACKET_TO_FUZZ_VERS);
  CMC_ERRB_ABLE(cmc_buff_pack(buff, data, size), goto err_pre_packet;);
  PACKET_TO_FUZZ packet =
      CMC_ERRB_ABLE(PACKET_TO_FUZZ_UNPACK(buff), goto err_pre_packet;);
  cmc_buff *tmp = cmc_buff_init(PACKET_TO_FUZZ_VERS);
  CMC_ERRB_ABLE(PACKET_TO_FUZZ_PACK(tmp, &packet),
                goto err_after_tmp);
  if (!cmc_buff_compare(buff, tmp)) {
    puts("not eqal!");
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
  puts("failed!");
  return 1;
}
