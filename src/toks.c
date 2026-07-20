#include <stddef.h>

char const * const tok_strs[] = {
#define tk(id) #id,
#include "toks.h"
#undef tk
    NULL,
};
