#include <cmc/err.h>

// NOLINTNEXTLINE(readability-redundant-declaration): extern inline
extern inline const char *cmc_err_as_str(cmc_err err);

const char *const cmc_err_strings[cmc_err_enum_size] = {
#define X(ERR) #ERR,
    CMC_ERR_ENUM_VALUES_X
#undef X
};
