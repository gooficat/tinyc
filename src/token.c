char const *const TOKENS[] = {
#define TK(id, str) str,
#include "token.h"
#undef TK
};
