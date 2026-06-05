#include "tree.h"
#include <corecrt_malloc.h>

struct ast tree;

static void init_scope(struct scope *scope, struct scope *parnt) {
	scope->parnt = parnt;
	scope->syms = malloc(1);
	scope->num_syms = 0;
	scope->nodes = malloc(1);
	scope->num_nodes = 0;
}

void tree_parse(void) {
	tree.typ = AstScope;
	init_scope(&tree.val.scope, NULL);
}

void tree_print(void) {
	switch (tree.typ) {
	case AstNone:
	case AstScope:
	case AstBinOp:
	case AstUnOp:
	case AstOrder:
	case AstFunc:
	case AstConst:
		break;
	}
}
