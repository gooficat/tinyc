#include "parse/ctx.h"
#include "parse/lex/lexer.h"
#include "parse/lex/toks.h"
#include "parse/tree/parse.h"
#include "strucs/value.h"

int main() {
	struct parse_ctx ctx;
	// init_tok_maps();
	ctx_init(&ctx, fopen("./test/1.c", "r"));
	gen_tree(&ctx);
}
