#include "cmc/buff.h"
#include "cmc/err.h"
#include <uuid/uuid.h>

#define UUID_SIZE (16)

// This only exists so i can return a uuid
typedef struct {
  uuid_t uuid;
} cmc_uuid;

cmc_err cmc_pack_uuid(cmc_buff *buff, cmc_uuid data);
cmc_uuid cmc_unpack_uuid(cmc_span *buff, cmc_err *err);
