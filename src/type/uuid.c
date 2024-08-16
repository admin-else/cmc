#include <cmc/type/uuid.h>
#include <cmc/buff.h>
#include <cmc/err.h>
#include <cmc/err_macros.h>
#include <string.h>

cmc_err cmc_pack_uuid(cmc_buff *buff, cmc_uuid data) {
  return cmc_pack(buff, data.uuid, UUID_SIZE);
}

cmc_uuid cmc_unpack_uuid(cmc_span *buff, cmc_err *err) {
  void *uuid_raw = CMC_ERRP_ABLE(cmc_unpack(buff, UUID_SIZE, err), return (cmc_uuid){0})
  cmc_uuid uuid = {0};
  memcpy(uuid.uuid, uuid_raw, UUID_SIZE);
  return uuid;
}
