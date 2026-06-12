#include "value.h"
#include "error.h"
#include "parse/ctx.h"
#include "utils/pstr.h"
#include "utils/vector.h"
#include <ctype.h>
#include <stdlib.h>

size_t gen_constant(struct parse_ctx *ctx) {
	size_t i = vec_len(ctx->constants);
	vec_grow(ctx->constants, 1);
	if (*ctx->lexer.skr == '"') {
		size_t j = 0;
		while (ctx->lexer.skr[j] != '\\' && ctx->lexer.skr[++j] != '"')
			;
		pstr_dup(&ctx->constants[i].s, ++ctx->lexer.skr, 0, j);
		ctx->lexer.skr += j;
		return i;
	}
	if (isdigit(*ctx->lexer.skr)) {
		char const *check = ctx->lexer.skr;
		while (isdigit(*++check))
			;
		if (*check == '.') {
			ctx->constants[i].type = C_CONST_FLOAT;
			ctx->constants[i].f = strtold(ctx->lexer.skr, &ctx->lexer.skr);
		}
		ctx->constants[i].type = C_CONST_INT;
		ctx->constants[i].i = strtoll(ctx->lexer.skr, &ctx->lexer.skr, 0);
		return i;
	}

	internal_error();
}
