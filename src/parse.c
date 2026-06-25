#include "lexer.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include <stddef.h>

ast_scope_s root;
ast_scope_s *curr_scop;

void handle_decl(void) {
}

void gen_expr(ast_node_s *node) {
}

size_t add_sym(c_sym_s *sym) {
	size_t i = vec_len(curr_scop->symbols);
	curr_scop->symbols = vec_grow(curr_scop->symbols, 1);
	curr_scop->symbols[i] = *sym;
	return i;
}

void add_node(ast_node_s *node) {
	size_t i = vec_len(curr_scop->children);
	curr_scop->children = vec_grow(curr_scop->children, 1);
	curr_scop->children[i] = *node;
}

void parse_tree(void) {
	root.parent = NULL;
	root.children = vec_init(ast_node_s);
	root.labels = vec_init(char *);
	curr_scop = &root;

	while (tok.type != TOK_EOF) {
		if (tok_is_decl()) {
			handle_decl();
		} else {
			ast_node_s expr;
			gen_expr(&expr);
			add_node(&expr);
		}
	}
}
