#include "parse.h"
#include "strucs/nodes.h"
#include "utils/arena.h"
#include <stdlib.h>

static void gen_scope(struct ast_scope *scope, struct lexer *lexer) {
}

void gen_tree(struct ast_node *node, struct lexer *lexer) {
	node->type = AST_SCOPE;
	node->val = arena_alloc(sizeof(struct ast_scope));
	gen_scope(node->val, lexer);
}
