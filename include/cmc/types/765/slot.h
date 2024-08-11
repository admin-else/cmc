

#include <cmc/type/c_types.h>

typedef struct {

  bool present;

  union {

    false;

    struct {

      cmc_varint itemId;

      int8_t itemCount;

      /* TODO: ADD THIS */ struct cmc_nbt_anon nbtData;

    } true;
  };

} cmc_765_slot;
