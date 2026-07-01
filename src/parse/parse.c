#include "lexer.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include <stddef.h>

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

void handle_stmt(void) {
	if (tok_is_decl()) {
		handle_decl();
	} else {
		ast_node_s expr;
		gen_expr(&expr);
		add_node(&expr);
	}
}

void parse_tree(void) {
	root.val.scope.parent = NULL;
	root.val.scope.children = vec_init(ast_node_s);
	root.val.scope.symbols = vec_init(c_sym_s);
	curr_scop = &root;

	while (tok.type != TOK_EOF) {
		handle_stmt();
	}
}
