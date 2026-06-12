#include "ctx.h"
#include "parse/lex/lexer.h"
#include "strucs/value.h"
#include "utils/pstr.h"

void ctx_init(struct parse_ctx *ctx, FILE *file) {
	lexer_init(&ctx->lexer, file);
	ctx->constants = vec_init(struct c_const);
	ctx->identifiers = vec_init(struct pstr);
	ctx->arena = arena_bootstrap(0xFFFF);
	ctx->current = &ctx->scope;
	lexer_next(ctx);
}
