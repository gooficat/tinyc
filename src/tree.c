#include "tree.h"
#include "dict.h"
#include <corecrt_malloc.h>
#include <stdio.h>

struct ast tree;

struct scope *current;

static void init_scope(struct scope *scope, struct scope *parnt) {
	scope->parnt = parnt;
	scope->syms = malloc(1);
	scope->num_syms = 0;
	scope->nodes = malloc(1);
	scope->num_nodes = 0;
}

static void handle_stmt(void) {
}

void tree_parse(void) {
	tree.typ = AstScope;
	init_scope(&tree.val.scope, NULL);
	current = &tree.val.scope;
}

/*



*/

static void print_node(struct ast *node);

static void print_scope(struct scope *scope) {
	size_t i;
	puts("Scope");
	for (i = 0; i < scope->num_syms; ++i) {
		printf("Symbol %s\n", scope->syms[i].name);
	}
	for (i = 0; i < scope->num_nodes; ++i) {
		print_node(scope->nodes + i);
	}
}

static void print_node(struct ast *node) {
	switch (node->typ) {
	case AstNone:
		puts("Error: No node\n");
		break;
	case AstScope:
		print_scope(&node->val.scope);
		break;
	case AstBinOp:
		printf("Operation: %s\nLeft:\n", OPERATORS[node->val.binop.operator]);
		print_node(node->val.binop.left);
		printf("Right:\n");
		print_node(node->val.binop.right);
		break;
	case AstUnOp:
		printf("Operation: %s\n", OPERATORS[node->val.unop.operator]);
		print_node(node->val.unop.node);
		break;
	case AstOrder:
		printf("Order\n");
		break;
	case AstFunc:
		printf("Function\n");
		printf("Named %s\n", node->val.func.sym->name);
		print_scope(&node->val.func.body);
		break;
	case AstConst:
		printf("Constant of type %i\n", node->val.cnst->typ);
		break;
	}
}

void tree_print(void) {
	print_node(&tree);
}
