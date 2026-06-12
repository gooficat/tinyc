#ifndef __ERROR__H__
#define __ERROR__H__

#include "parse/ctx.h"
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void parse_error(struct parse_ctx *ctx);
noreturn void syntax_error(struct parse_ctx *ctx);
noreturn void internal_error();

#endif //!__ERROR__H__
