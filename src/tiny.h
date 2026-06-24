#ifndef __TINY__H__
#define __TINY__H__

typedef enum {
#define TK(id, str) id,
#include "token.h"
#undef TK
} tok_e;

#endif //!__TINY__H__
