#include "cmc/types/uuid.h"
#include "cmc/buff.h"
#include "cmc/err.h"
#include <string.h>

cmc_err cmc_pack_uuid(cmc_buff_packing *buff, cmc_uuid data) {
  return cmc_pack(buff, data.uuid, UUID_SIZE);
}

cmc_uuid cmc_unpack_uuid(cmc_buff_unpacking *buff, cmc_err *err) {
  void *uuid_raw = cmc_unpack(buff, UUID_SIZE, err);
  if (*err != CMC_ERR_NO) {
    return (cmc_uuid){0};
  }
  cmc_uuid uuid = {0};
  memcpy(uuid.uuid, uuid_raw, UUID_SIZE);
  return uuid;
}

