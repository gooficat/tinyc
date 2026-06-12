#ifndef __ERROR__H__
#define __ERROR__H__

#include "parse/ctx.h"

void parse_error(struct parse_ctx *ctx);
void syntax_error(struct parse_ctx *ctx);
void internal_error();

#endif //!__ERROR__H__
