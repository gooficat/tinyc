#include "parse/ctx.h"
#include "parse/lex/lexer.h"
#include "parse/lex/toks.h"
#include "strucs/value.h"

int main() {
	struct parse_ctx ctx;
	// init_tok_maps();
	ctx_init(&ctx, fopen("./test/1.c", "r"));
	while (ctx.lexer.tok.type != TK_NULL) {
		fprintf(stdout, "Token %i %zu\n", ctx.lexer.tok.type, ctx.lexer.tok.val);
		lexer_next(&ctx);
	}
}
