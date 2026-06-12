#ifndef __CTX__H__
#define __CTX__H__

#include "parse/lex/lexer.h"
#include "strucs/nodes.h"
#include "utils/arena.h"

struct parse_ctx {
	struct lexer lexer;
	struct ast_scope scope;
	struct ast_scope *current;
	struct arena arena;
};

#endif //!__CTX__H__
