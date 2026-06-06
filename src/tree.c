#include "tree.h"
#include "dict.h"
#include "lexer.h"
#include "val.h"
#include <corecrt_malloc.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast tree;
struct scope *cur;

static int types_compatible(struct typ *a, struct typ *b) {
	return 1; /* allow all for now :|< */
}

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
	size_t l = cur->num_nodes;
	cur->nodes = realloc(cur->nodes, ++cur->num_nodes);
	gen_node(&cur->nodes[l]);
}

static void affect_typ(struct typ *typ) {
	switch (tok.idx) {
	case KwInt:
		typ->typ = TypeInt;
		break;
	case KwSigned:
		if (!typ->val.i.signedness) {
			typ->val.i.signedness = 2;
			break;
		}
		goto err;
	case KwUnsigned:
		if (!typ->val.i.signedness) {
			typ->val.i.signedness = 2;
			break;
		}
		goto err;
	case KwShort:
		if (!typ->len) {
			typ->len = sizeof(short);
			break;
		}
		goto err;
	case KwChar:
		if (!typ->len) {
			typ->len = sizeof(char);
			break;
		}
		goto err;
	case KwLong:
		if (!typ->len) {
			typ->len = sizeof(long);
			break;
		}
		/*else if (typ->len == sizeof(long)) {
			typ->typ = sizeof(long long);
		}*/
		goto err;
	case KwFloat:
		if (!typ->typ) {
			typ->typ = TypeFloat;
			if (!typ->len) {
				typ->len = sizeof(float);
				break;
			}
		}
		goto err;
	case KwDouble:
	case KwVoid:
	case KwStruct:
	case KwUnion:
	case KwEnum:
	case KwVolatile:
	case KwRegister:
	case KwConst:
	case KwAuto:
	case KwStatic:
	case KwExtern:
	case KwInline:
		fprintf(stderr, "Error: unimplemented symbol in type specifier\n");
		exit(EXIT_FAILURE);

	default:
		fprintf(stderr, "Error: wrong symbol in type specifier\n");
		exit(EXIT_FAILURE);
	}
	lxr_next();
	while (tok.typ == TokOper && tok.idx == OpMul) {
		struct typ *prv = malloc(sizeof(struct typ));
		*prv = *typ;
		typ->typ = TypePtr;
		typ->val.p = prv;
		lxr_next();
	}
	return;
err:
	fprintf(stderr, "Malformed type\n");
	exit(EXIT_FAILURE);
}

static void gen_expr(struct ast *node) {
	if (tok.typ == TokOper) {
		/*gen_unary*/
	}
}

static void gen_binop(struct ast *binop, struct ast *left) {
	binop->typ = AstBinOp;
	lxr_next();
	binop->val.binop.left = left;
	binop->val.binop.operator = tok.idx;
	binop->val.binop.right = malloc(sizeof(struct ast));
}

static void handle_decl(void) {
	struct sym *sym;
	bool is_td;
	is_td = false;
	sym = malloc(sizeof(struct sym));
	sym->typ = malloc(sizeof(struct typ));
	memset(sym->typ, 0, sizeof(struct typ));
	while (tok.typ == TokKword) {
		if (tok.idx == KwTypedef) {
			is_td = true;
			lxr_next();
		}
		affect_typ(sym->typ);
	}
	if (is_td) {
		/*cur->syms.*/
		puts("Typedef unimplemented\n");
		exit(EXIT_FAILURE);
		/*return;*/
	}
	if (tok.typ != TokIdent) {
		fprintf(stderr, "Unexpected symbol after type");
	}
	sym->name = idens[tok.idx];
	lxr_next();
	for (;;) {
		if (tok.idx == PnParenL) {
			sym->typ->args = malloc(1);
			sym->typ->num_args = 0;
			lxr_next();
			while (tok.typ != TokPunc || tok.idx != PnParenR) {
				size_t old = sym->typ->num_args;
				sym->typ->args = realloc(sym->typ->args, ++sym->typ->num_args * sizeof(struct sym));
				sym->typ->args[old].typ = malloc(sizeof(struct typ));

				while (tok.typ == TokKword) {
					affect_typ(sym->typ->args[old].typ);
				}
				if (tok.typ != TokIdent) {
					sym->typ->args[old].name = NULL;
				} else {
					sym->typ->args[old].name = idens[tok.idx];
					lxr_next();
				}
				if (tok.typ == TokPunc) {
					if (tok.idx == PnParenR) {
						continue;
					}
					if (tok.idx != PnComma) {
						fprintf(stderr, "Expected comma\n");
						exit(EXIT_FAILURE);
					}
				}
				lxr_next();
				if (tok.typ == TokPunc && tok.idx == PnBraceL) {
				}
			}
			lxr_next();
			if (tok.typ == TokPunc) {
				if (tok.idx == PnSemi) {
					lxr_next();
					return;
				}
			}
			lxr_next();
		}
		if (tok.typ == TokOper && tok.idx == OpAss) {
			size_t old = cur->num_nodes;
			cur->nodes = realloc(cur->nodes, ++cur->num_nodes);
			lxr_next();
			cur->nodes[old].typ = AstBinOp;
			cur->nodes[old].val.binop.left = malloc(sizeof(struct ast));
			cur->nodes[old].val.binop.left->typ = AstRef;
			cur->nodes[old].val.binop.left->val.ref = sym;
			cur->nodes[old].val.binop.operator = tok.idx; /*OpAss*/
			lxr_next();
			cur->nodes[old].val.binop.right = malloc(sizeof(struct ast));
			gen_expr(cur->nodes[old].val.binop.right);
			return;
		}
		if (tok.idx != PnComma) {
			break;
		}
		lxr_next();
	}
	if (tok.idx != PnSemi) {
		fprintf(stderr, "Unexpected symbol following symbol declaration\n");
		exit(EXIT_FAILURE);
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
	cur = &tree.val.scope;
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
		return;
	case AstScope:
		print_scope(&node->val.scope);
		return;
	case AstBinOp:
		printf("Operation: %s\nLeft:\n", OPERATORS[node->val.binop.operator]);
		print_node(node->val.binop.left);
		printf("Right:\n");
		print_node(node->val.binop.right);
		return;
	case AstUnOp:
		printf("Operation: %s\n", OPERATORS[node->val.unop.operator]);
		print_node(node->val.unop.node);
		return;
	case AstOrder:
		printf("Order\n");
		return;
	case AstFunc:
		printf("Function\n");
		printf("Named %s\n", node->val.func.sym->name);
		print_scope(&node->val.func.body);
		return;
	case AstCond:
		printf("Conditional %i\n", node->val.cond.typ);
		printf("Condition:\n");
		print_node(node->val.cond.cond);
		printf("Body:\n");
		print_node(node->val.cond.body);
		if (node->val.cond.els) {
			printf("Else:\n");
			print_node(node->val.cond.els);
		}
		return;
	case AstConst:
		printf("Constant of type %i\n", node->val.cnst->typ);
		return;
	}
}

void tree_print(void) {
	print_node(&tree);
}
