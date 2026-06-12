#include "parse/ctx.h"
#include "parse/lex/toks.h"
#include "strucs/value.h"

int main() {
	struct parse_ctx ctx;
	// init_tok_maps();
	ctx_init(&ctx, fopen("./test/1.c", "r"));
}
