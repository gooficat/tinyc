#include "gen.h"
#include "mem.h"
#include "parse/parse.h"
#include "tree.h"
#include <stdarg.h>
#include <stdio.h>
#include <vadefs.h>

static FILE *file;

bool is_top_scope(void) {
	return !curr_scop->val.scope.parent;
}

void emit(char const *str, ...) {
	va_list va;
	va_start(va, str);
	fprintf(file, str, va);
	fflush(file);
	va_end(va);
}

void codegen_init(FILE *file_) {
	file = file_;
}

void codegen_scope(ast_node_s *node) {
	curr_scop = node;
	codegen_prep_frame(node);
}

void codegen_tree(void) {
	codegen_scope(&root);
}

void codegen_close(void) {
}
