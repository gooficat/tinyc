#include "tree.h"
#include "dict.h"
#include "err.h"
#include "gen.h"
#include "lexer.h"
#include "val.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast tree;
struct scope *cur;

static void handle_stmt(void);

static int types_compatible(struct typ *a, struct typ *b) {
	return 1; /* allow all for now >:| */
}

static struct sym *find_sym(char const *name, struct scope *cur) {
	size_t i;
	for (i = 0; i < cur->num_syms; ++i) {
		if (!strcmp(cur->syms[i].name, name)) {
			return &cur->syms[i];
		}
	}
	if (cur->parnt) {
		return find_sym(name, cur->parnt);
	}
	return NULL;
}

static void init_scope(struct scope *scope, struct scope *parnt) {
	scope->parnt = parnt;
	scope->syms = malloc(1);
	scope->num_syms = 0;
	scope->nodes = malloc(1);
	scope->num_nodes = 0;
}

void gen_node(struct ast *node) {
	switch (tok.typ) {
	case TokNone:
		parse_panic("Cannot gen node from nothing");
	case TokConst:
		node->typ = AstConst;
		node->val.cnst = &cnsts[tok.idx];
		lxr_next();
		break;
	case TokKword:
		/* node->typ = AstCast;*/
		parse_panic("Casting is not implemented yet\n");
		break;
	case TokIdent:
		node->typ = AstRef;
		node->val.ref = find_sym(idens[tok.idx], cur);
		lxr_next();
		break;
	case TokPunc: {
		parse_panic("Punc is not implemented yet");
		break;
	}
	case TokOper:
		parse_panic("Operator is not implemented yet");
		break;
	}
	if (tok.typ == TokPunc) {
		switch (tok.idx) {
		case PnParenL: {
			{
				struct ast old = *node;
				dbg_print("This is a call\n");
				node->typ = AstCall;
				node->val.call.of = malloc(sizeof(struct ast));
				*node->val.call.of = old;
				node->val.call.args.nodes = malloc(1);
				node->val.call.args.num_nodes = 0;
			}
			lxr_next();
			if (tok.typ != TokPunc || tok.idx != PnParenR) {
			rpt:
				size_t old = node->val.call.args.num_nodes;
				node->val.call.args.nodes = realloc(node->val.call.args.nodes, ++node->val.call.args.num_nodes * sizeof(struct ast));
				gen_node(&node->val.call.args.nodes[old]);
				if (tok.typ == TokPunc && tok.idx == PnComma) {
					lxr_next();
					goto rpt;
				}
				if (tok.typ != TokPunc || tok.idx != PnParenR) {
					parse_panic("Unexpected symbol in function call");
				}
			}
			lxr_next();
		} break;
		case PnComma:
			/*TODO!!!! Comma lists are a language construct*/
			break;
		}
	}
}

static void handle_expr(void) {
	size_t l = cur->num_nodes;
	cur->nodes = realloc(cur->nodes, ++cur->num_nodes * sizeof(struct ast));
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
			typ->len = SHORT_SIZE;
			break;
		}
		goto err;
	case KwChar:
		if (!typ->typ) {
			typ->typ = TypeChar;
			if (!typ->len) {
				typ->len = CHAR_SIZE;
				break;
			}
		}
		goto err;
	case KwLong:
		if (!typ->len) {
			typ->len = LONG_SIZE;
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
				typ->len = FLOAT_SIZE;
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
		parse_panic("Error: unimplemented symbol in type specifier");

	default:
		parse_panic("Error: wrong symbol in type specifier");
	}
	lxr_next();
	while (tok.typ == TokOper && tok.idx == OpMul) {
		struct typ *prv = malloc(sizeof(struct typ));
		*prv = *typ;
		typ->typ = TypePtr;
		typ->val.p = prv;
		typ->len = 4; /*for now*/
		lxr_next();
	}
	return;
err:
	parse_panic("Malformed type");
}

static void gen_binop(struct ast *binop, struct ast *left) {
	binop->typ = AstBinOp;
	lxr_next();
	binop->val.binop.left = left;
	binop->val.binop.operator = tok.idx;
	binop->val.binop.right = malloc(sizeof(struct ast));
}

static void parse_scope(void) {
	dbg_print("Parsing a scope\n");
	lxr_next();
	while (tok.typ != TokPunc || tok.idx != PnBraceR) {
		if (tok.typ == TokNone) {
			parse_panic("Unexpected end of input\n");
		}
		if (tok.typ == TokPunc && tok.idx == PnSemi) {
			lxr_next();
			continue;
		}
		handle_stmt();
	}
	lxr_next();
}

static void handle_decl(void) {
	struct sym *sym;
	unsigned char is_td;
	size_t old = cur->num_syms;
	is_td = 0;
	cur->syms = realloc(cur->syms, ++cur->num_syms * sizeof(struct sym));
	sym = &cur->syms[old];
	sym->typ = malloc(sizeof(struct typ));
	memset(sym->typ, 0, sizeof(struct typ));
	dbg_print("PArsing decl\n");
	while (tok.typ == TokKword) {
		if (tok.idx == KwTypedef) {
			is_td = 1;
			lxr_next();
		}
		affect_typ(sym->typ);
	}
	if (is_td) {
		/*cur->syms.*/
		parse_panic("Typedef unimplemented");
		/*return;*/
	}
	if (tok.typ != TokIdent) {
		parse_panic("Unexpected symbol after type");
	}
	sym->name = idens[tok.idx];
	lxr_next();
	for (;;) {
		if (tok.idx == PnParenL) {
			dbg_print("Function decl\n");
			sym->typ->args = malloc(1);
			sym->typ->num_args = 0;
			lxr_next();
			while (tok.typ != TokPunc || tok.idx != PnParenR) {
				size_t old = sym->typ->num_args;
				dbg_print("Arg\n");
				sym->typ->args = realloc(sym->typ->args, ++sym->typ->num_args * sizeof(struct sym));
				sym->typ->args[old].typ = malloc(sizeof(struct typ));
				memset(sym->typ->args[old].typ, 0, sizeof(struct typ));
				while (tok.typ == TokKword) {
					dbg_print("About to parse type\n");
					affect_typ(sym->typ->args[old].typ);
				}
				dbg_print("Finished parse type\n");
				if (tok.typ != TokIdent) {
					sym->typ->args[old].name = NULL;
				} else {
					sym->typ->args[old].name = idens[tok.idx];
					lxr_next();
				}
				if (tok.typ == TokPunc) {
					if (tok.idx == PnParenR) {
						break;
					}
					if (tok.idx != PnComma) {
						parse_panic("Expected comma");
					}
				}
				lxr_next();
			}
			lxr_next();
			if (tok.typ == TokPunc && tok.idx == PnBraceL) {
				size_t old;

				old = cur->num_nodes;
				cur->nodes = realloc(cur->nodes, ++cur->num_nodes * sizeof(struct ast));

				cur->nodes[old].typ = AstFunc;
				cur->nodes[old].val.func.sym = sym;
				cur->nodes[old].val.func.body.parnt = cur;
				cur->nodes[old].val.func.body.nodes = malloc(1);
				cur->nodes[old].val.func.body.num_nodes = 0;
				cur->nodes[old].val.func.body.syms = sym->typ->args;
				cur->nodes[old].val.func.body.num_syms = sym->typ->num_args;
				cur->nodes[old].val.func.body.parnt = cur;

				cur = &cur->nodes[old].val.func.body;
				parse_scope();
				cur = cur->parnt;

				goto finish;
			}
			if (tok.typ == TokPunc) {
				if (tok.idx == PnSemi) {
					lxr_next();
					goto finish;
				}
			}
		} else if (tok.typ == TokOper && tok.idx == OpAss) {
			size_t old = cur->num_nodes;
			dbg_print("Assignment\n");
			cur->nodes = realloc(cur->nodes, ++cur->num_nodes * sizeof(struct ast));
			lxr_next();
			cur->nodes[old].typ = AstBinOp;
			cur->nodes[old].val.binop.left = malloc(sizeof(struct ast));
			cur->nodes[old].val.binop.left->typ = AstRef;
			cur->nodes[old].val.binop.left->val.ref = sym;
			cur->nodes[old].val.binop.operator = tok.idx; /*OpAss*/
			lxr_next();
			cur->nodes[old].val.binop.right = malloc(sizeof(struct ast));
			gen_node(cur->nodes[old].val.binop.right);
			goto finish;
		}
		if (tok.idx != PnComma) {
			dbg_print("Not comma\n");
			break;
		}
		lxr_next();
	}
	if (tok.idx != PnSemi) {
		parse_panic("Unexpected symbol following symbol declaration");
	}

finish:
	return;
}
static void handle_ordr(void) {
	size_t old = cur->num_nodes;
	cur->nodes = realloc(cur->nodes, ++cur->num_nodes * sizeof(struct ast));
	cur->nodes[old].typ = AstOrder;
	switch (tok.idx) {
	case KwReturn:
		cur->nodes[old].val.order.ordr = OrderReturn;
		lxr_next();
		cur->nodes[old].val.order.val.node = malloc(sizeof(struct ast));
		gen_node(cur->nodes[old].val.order.val.node);
		break;
	case KwBreak:
		cur->nodes[old].val.order.ordr = OrderBreak;
		lxr_next();
		break;
	case KwContinue:
		cur->nodes[old].val.order.ordr = OrderContinue;
		lxr_next();
		break;
	case KwGoto:
		cur->nodes[old].val.order.ordr = OrderGoto;
		lxr_next();
		/* TODO add label namespace or emulated namespace with flag */
		parse_panic("Goto unimplemented");
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
			parse_panic("Else statement with no preceding if!");
		}
		if (tok.idx < KwSizeof) {
			parse_panic("This type of token cannot appear outside of switches!");
		}
		parse_panic("Sizeof unimplemented");
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
	dbg_print("Finished parsing\n");
}
