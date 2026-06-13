#ifndef __CTX__H__
#define __CTX__H__

#include "parse/lex/lexer.h"
#include "strucs/nodes.h"
#include "strucs/value.h"
#include "utils/arena.h"

struct parse_ctx {
	struct lexer lexer;
	struct ast_scope scope;
	struct ast_scope *current;
	struct arena arena;

	vec(struct c_const) constants;
	vec(char *) identifiers;
};

void ctx_init(struct parse_ctx *ctx, FILE *file);

#endif //!__CTX__H__
