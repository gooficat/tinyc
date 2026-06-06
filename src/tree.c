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
		}
		affect_typ(sym->typ);
		lxr_next();
		if (tok.typ == TokOper && tok.idx == OpMul) {
			struct typ *old = sym->typ;
			sym->typ = malloc(sizeof(struct typ));
			sym->typ->typ = TypePtr;
			sym->typ->val.p = old;
			lxr_next();
		}
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
	if (tok.typ == TokPunc) {
		if (tok.idx == PnSemi) {
			return;
		}
		if (tok.idx == PnParenL) {
			struct ast fn;
			fn.typ = AstFunc;
			fn.val.func.sym = sym;
			init_scope(&fn.val.func.body, cur);
			lxr_next();
			while (tok.typ != TokPunc || tok.idx != PnParenR) {
				size_t old = fn.val.scope.num_syms;
				fn.val.scope.syms = realloc(fn.val.scope.syms, ++fn.val.scope.num_syms * sizeof(struct sym));
				fn.val.scope.syms[old].typ = malloc(sizeof(struct typ));

				while (tok.typ == TokKword) {
					affect_typ(fn.val.scope.syms[old].typ);
					lxr_next();
					if (tok.typ == TokOper && tok.idx == OpMul) {
						struct typ *old = fn.val.scope.syms[old]->typ;
						fn.val.scope.syms[old].typ = malloc(sizeof(struct typ));
						fn.val.scope.syms[old].typ->typ = TypePtr;
						fn.val.scope.syms[old].typ->val.p = old;
						lxr_next();
					}
				}
				if (tok.typ != TokIdent) {
					fn.val.scope.syms[old].name = NULL;
				} else {
					fn.val.scope.syms[old].name = idens[tok.idx];
					lxr_next();
				}
				if (tok.typ == TokPunc && tok.idx != PnComma) {
					fprintf(stderr, "Expected comma\n");
					exit(EXIT_FAILURE);
				}
			}
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
			cur->nodes[old].val.binop.right = malloc(sizeof(struct ast));
			gen_expr(cur->nodes[old].val.binop.right);
			return;
		}
		fprintf(stderr, "Unexpected symbol following symbol declaration\n");
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

	static void print_node(struct ast * node);

	static void print_scope(struct scope * scope) {
		size_t i;
		puts("Scope");
		for (i = 0; i < scope->num_syms; ++i) {
			printf("Symbol %s\n", scope->syms[i].name);
		}
		for (i = 0; i < scope->num_nodes; ++i) {
			print_node(scope->nodes + i);
		}
	}

	static void print_node(struct ast * node) {
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
