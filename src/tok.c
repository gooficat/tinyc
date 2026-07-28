#include <stddef.h>

char const *const TOKS[] = {
#define T(id, str) str,
#include "tok.h"
#undef T
    NULL,
};
