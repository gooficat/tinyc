#include "error/error.h"
#include "lexer.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include <stddef.h>
#include <stdlib.h>

ast_node_s root;
ast_node_s *curr_scop;

void handle_decl(void);
void gen_expr(ast_node_s *node);

size_t add_sym(c_sym_s *sym) {
	size_t i = vec_len(curr_scop->val.scope.symbols);
	curr_scop->val.scope.symbols = vec_grow(curr_scop->val.scope.symbols, 1);
	curr_scop->val.scope.symbols[i] = *sym;
	return i;
}

void add_node(ast_node_s *node) {
	size_t i = vec_len(curr_scop->val.scope.children);
	curr_scop->val.scope.children = vec_grow(curr_scop->val.scope.children, 1);
	curr_scop->val.scope.children[i] = *node;
}

void init_scope(ast_node_s *scope) {
	scope->type = AST_SCOPE;
	scope->val.scope.children = vec_init(ast_node_s);
	scope->val.scope.parent = curr_scop;
	scope->val.scope.symbols = vec_init(c_sym_s);
}

void gen_order(ast_node_s *order) {
	order->type = AST_ORDER;
	order->val.order.type = tok.type;
	lexer_next();
	switch (order->val.order.type) {
	case TOK_KW_RETURN:
		order->val.order.val.expr = malloc(sizeof(ast_node_s));
		gen_expr(order->val.order.val.expr);
		break;
	case TOK_KW_BREAK:
	case TOK_KW_CONTINUE:
		break;
	case TOK_KW_GOTO:
		if (tok.type != TOK_IDENT) {
			error(ERR_SYNTAX, "Can only jump to a label");
		}
		order->val.order.val.idx = tok.val;
		lexer_next();
		break;
	}
}

void handle_stmt(void) {
	if (tok_is_decl()) {
		handle_decl();
	} else {
		ast_node_s node;
		if (tok_is_kword()) {
			gen_order(&node);
		} else {
			gen_expr(&node);
		}
		add_node(&node);
	}
}

void parse_tree(void) {
	root.val.scope.parent = NULL;
	root.val.scope.children = vec_init(ast_node_s);
	root.val.scope.symbols = vec_init(c_sym_s);
	curr_scop = &root;

	while (tok.type != TOK_EOF) {
		if (tok.type == TOK_SEMI) {
			lexer_next();
		} else {
			handle_stmt();
		}
	}
}
