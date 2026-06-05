#include "tree.h"
#include "dict.h"
#include "lexer.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

struct ast tree;

struct scope *current;

static void init_scope(struct scope *scope, struct scope *parnt) {
	scope->parnt = parnt;
	scope->syms = malloc(1);
	scope->num_syms = 0;
	scope->nodes = malloc(1);
	scope->num_nodes = 0;
}

void gen_node(struct ast *node) {
	(void)node;
}

static void handle_expr(void) {
	size_t l = current->num_nodes;
	current->nodes = realloc(current->nodes, ++current->num_nodes);
	gen_node(&current->nodes[l]);
}

static void handle_decl(void) {
}

static void handle_ordr(void) {
}

static void handle_cond(void) {
}

static void handle_stmt(void) {
	if (tok.typ == TokKword) {
		if (tok.idx <= KwTypedef) {
			handle_decl();
			return;
		}
		if (tok.idx <= KwGoto) {
			handle_ordr();
			return;
		}
		if (tok.idx <= KwSwitch) {
			handle_cond();
			return;
		}
		if (tok.idx < KwSizeof) {
			fprintf(stderr, "This type of token cannot appear outside of switches!\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "Sizeof unimplemented\n");
		exit(EXIT_FAILURE);
	}
	handle_expr();
}

void tree_parse(void) {
	tree.typ = AstScope;
	init_scope(&tree.val.scope, NULL);
	current = &tree.val.scope;
	while (tok.typ) {
		handle_stmt();
	}
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
