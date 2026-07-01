#include "error/error.h"
#include "lexer.h"
#include "tree.h"
#include <stdlib.h>

void gen_expr(ast_node_s *node) {
	if (tok.type == TOK_IDENT) {

	} else if (tok_is_op_pref()) {
		node->type = AST_UN_OP;
		node->val.un_op.op = tok.type;
		lexer_next();
		node->val.un_op.is_post = false;
		node->val.un_op.base = malloc(sizeof(ast_node_s));
		gen_expr(node->val.un_op.base);
	} else {
		switch (tok.type) {
		case TOK_PAREN_L:
		case TOK_BRACE_L:
		case TOK_BRACK_L:
			break;
		default:
			error(ERR_SYNTAX, "Unexpected token");
		}
	}
}
