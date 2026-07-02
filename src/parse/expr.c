#include "error/error.h"
#include "lexer.h"
#include "parse.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include <stdlib.h>

void gen_expr(ast_node_s *node);

inline void handle_paren(ast_node_s *node) {
	lexer_next();
	if (tok_is_decl()) {
		node->type = AST_CAST;
		while (tok_is_decl()) {
			mod_type(&node->val.cast.type);
		}
		if (tok.type != TOK_PAREN_R) {
			error(ERR_SYNTAX, "Unexpected token");
		}
		lexer_next();
		node->val.cast.val = malloc(sizeof(ast_node_s));
		gen_expr(node->val.cast.val);
	} else {
		gen_expr(node); // TODO! the atom of an expression
		lexer_next();
	}
}

inline void gen_scope(ast_node_s *node) {
	init_scope(node);
	curr_scop = node;

	curr_scop = curr_scop->val.scope.parent;

	while (tok.type != TOK_BRACE_R) {
		handle_stmt();
	}
	lexer_next();
}

void gen_atom(ast_node_s *node) {
	if (tok.type == TOK_IDENT) {
		node->type = AST_VREF;
		node->val.idx = tok.val;
		lexer_next();
	} else if (tok.type == TOK_VALUE) {
		node->type = AST_VALUE;
		node->val.idx = tok.val;
		lexer_next();
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
			handle_paren(node);
			break;
		case TOK_BRACE_L:
			gen_scope(node);
			break;
		default:
			error(ERR_SYNTAX, "Unexpected token");
		}
	}
}

void gen_list(ast_list_s *list) {
	list->elems = vec_init(ast_node_s);
	for (;;) {
		size_t idx = vec_len(list->elems);
		list->elems = vec_grow(list->elems, 1);
		gen_expr(list->elems + idx);
		if (tok.type != TOK_COMMA) {
			break;
		}
		lexer_next();
	}
}

void gen_expr(ast_node_s *node) {
	gen_atom(node);
	if (tok.type == TOK_PAREN_L) {
		ast_node_s *new = malloc(sizeof(ast_node_s));
		*new = *node;
		node->type = AST_CALL;
		node->val.call.of = new;
		lexer_next();
		gen_list(&node->val.call.args);
		if (tok.type != TOK_PAREN_R) {
			error(ERR_SYNTAX, "Expected right parentheses");
		}
		lexer_next();
	}
	if (tok.type == TOK_COMMA) {
		ast_node_s *new = malloc(sizeof(ast_node_s));
		*new = *node;
		node->type = AST_LIST;
		node->val.list.elems = vec_init(ast_node_s);
		do {
			size_t idx = vec_len(node->val.list.elems);
			lexer_next();
			node->val.list.elems = vec_grow(node->val.list.elems, 1);
			gen_expr(node->val.list.elems + idx);
		} while (tok.type == TOK_COMMA);
	}

	if (tok_is_bin_op()) {
		// TODO
		error(ERR_INTERNAL, "Binary op unimplemented");
	}
}
