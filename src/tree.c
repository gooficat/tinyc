#include "tree.h"
#include "dict.h"
#include "lexer.h"
#include "val.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void affect_typ(struct typ *typ) {
	switch (tok.idx) {
	case KwInt:
		typ->typ = TypeInt;
		return;
	case KwSigned:
		if (!typ->val.i.signedness) {
			typ->val.i.signedness = 2;
			return;
		}
		break;
	case KwUnsigned:
		if (!typ->val.i.signedness) {
			typ->val.i.signedness = 2;
			return;
		}
		break;
	case KwShort:
		if (!typ->len) {
			typ->len = sizeof(short);
			return;
		}
		break;
	case KwChar:
		if (!typ->len) {
			typ->len = sizeof(char);
			return;
		}
		break;
	case KwLong:
		if (!typ->len) {
			typ->len = sizeof(long);
			return;
		}
		/*else if (typ->len == sizeof(long)) {
			typ->typ = sizeof(long long);
		}*/
		break;
	case KwFloat:
		if (!typ->typ) {
			typ->typ = TypeFloat;
			if (!typ->len) {
				typ->len = sizeof(float);
				return;
			}
		}
		break;
	case KwDouble:
		return;
	case KwVoid:
		return;
	case KwStruct:
		return;
	case KwUnion:
		return;
	case KwEnum:
		return;
	case KwVolatile:
		return;
	case KwRegister:
		return;
	case KwConst:
		return;
	case KwAuto:
		return;
	case KwStatic:
		return;
	case KwExtern:
		return;
	case KwInline:
		return;
	}
	fprintf(stderr, "Malformed type\n");
	exit(EXIT_FAILURE);
}

static void handle_decl(void) {
	struct typ *typ = malloc(sizeof(struct typ));
	bool is_td = false;
	memset(typ, 0, sizeof *typ);
	while (tok.typ == TokKword) {
		if (tok.idx == KwTypedef) {
			is_td = true;
		}
		affect_typ(typ);
		lxr_next();
		if (tok.typ == TokOper && tok.idx == OpMul) {
			struct typ *old = typ;
			typ = malloc(sizeof(struct typ));
			typ->typ = TypePtr;
			typ->val.p = old;
			lxr_next();
		}
	}
}

static void handle_ordr(void) {
	switch (tok.idx) {
	case KwReturn:
	case KwBreak:
	case KwContinue:
	case KwGoto:
		break;
	}
}

static void handle_cond(void) {
	switch (tok.idx) {
	case KwIf:
	case KwWhile:
	case KwDo:
	case KwFor:
	case KwSwitch:
		break;
	}
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
		if (tok.idx == KwElse) {
			fprintf(stderr, "Else statement with no preceding if!");
			exit(EXIT_FAILURE);
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
