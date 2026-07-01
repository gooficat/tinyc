#include "error/error.h"
#include "lexer.h"
#include "parse.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include "type.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

size_t add_sym(c_sym_s *sym);

static void mod_type(type_s *type) {
	switch ((int)tok.type) {
	case TOK_KW_INT:
		if (type->type != TYPE_NONE) {
			goto error;
		}
		type->type = TYPE_INT;
		break;
	case TOK_KW_CHAR:
		if (type->type != TYPE_NONE) {
			goto error;
		}
		type->type = TYPE_INT;
		type->info.igr.type = INT_CHAR;
		break;
	case TOK_KW_SHORT:
		if (type->type != TYPE_NONE) {
			goto error;
		}
		type->type = TYPE_INT;
		if (type->info.igr.type != INT_PLAIN) {
			goto error;
		}
		type->info.igr.type = INT_CHAR;
		break;
	case TOK_KW_LONG:
		if (type->type != TYPE_NONE) {
			goto error;
		}
		type->type = TYPE_INT;
		if (type->info.igr.type == INT_LONG_LONG) {
			goto error;
		}
		if (type->info.igr.type == INT_LONG) {
			type->info.igr.type = INT_LONG_LONG;
		} else {
			type->info.igr.type = INT_LONG;
		}
		break;
	case TOK_KW_DOUBLE:
		if (type->type != TYPE_NONE) {
		}
		if (type->info.igr.type == INT_LONG) {
			type->info.flt = FLT_LONG_DOUBLE;
		} else if (type->info.igr.type == INT_PLAIN) {
			type->info.flt = FLT_DOUBLE;
		} else {
			goto error;
		}
		type->type = TYPE_FLOAT;
		break;
	case TOK_KW_UNSIGNED:
		if (type->type == TYPE_NONE) {
			type->type = TYPE_INT;
		} else if (type->type != TYPE_INT) {
			goto error;
		}
		if (type->info.igr.sign != SIGN_UNKNOWN) {
			goto error;
		}
		type->info.igr.sign = SIGN_UNSIGNED;
		break;
	case TOK_KW_SIGNED:
		if (type->type == TYPE_NONE) {
			type->type = TYPE_INT;
		} else if (type->type != TYPE_INT) {
			goto error;
		}
		if (type->info.igr.sign != SIGN_UNKNOWN) {
			goto error;
		}
		type->info.igr.sign = SIGN_SIGNED;
		break;
	case TOK_KW_CONST:
		type->is_const = true;
		break;
	case TOK_KW_VOLATILE:
		type->is_volat = true;
		break;
	case TOK_KW_RESTRICT:
		type->is_restr = true;
		break;
	case TOK_KW_ENUM:
	case TOK_KW_STRUCT:
	case TOK_KW_UNION:
		// TODO
	default:
		goto error;
	}
	lexer_next();
	if (tok.type == TOK_OP_MUL) {
		type_s new_type;
		new_type.type = TYPE_PTR;
		new_type.info.ptr = malloc(sizeof(type_s));
		*new_type.info.ptr = *type;
		*type = new_type;
		lexer_next();
	}
	return;
error:
	error(ERR_SYNTAX, "Malformed type");
}

static void parse_params(vec(c_sym_s) * params) {
	lexer_next();
	if (tok.type == TOK_KW_VOID) {
		lexer_next();
	} else {
		for (;;) {
			c_sym_s *sym = &(*params[vec_len(params) - 1]);
			memset(sym, 0, sizeof(c_sym_s));

			while (tok_is_decl()) {
				if (tok_is_store()) {
					// TODO
				} else {
					mod_type(&sym->type);
				}
			}
			if (tok.type != TOK_IDENT) {
				error(ERR_SYNTAX, "Expected variable name");
			}
			sym->name = tok.val;
			lexer_next();
			if (tok.type == TOK_PAREN_R) {
				break;
			}
			if (tok.type != TOK_COMMA) {
				error(ERR_SYNTAX, "Expected comma");
			}
			lexer_next();
		}
	}
	if (tok.type != TOK_PAREN_R) {
		error(ERR_SYNTAX, "Expected parentheses");
	}
	lexer_next();
}

size_t gen_sym(type_s *type, storag_e stor) {
	c_sym_s sym;
	sym.storage = stor;
	sym.name = tok.val;
	lexer_next();
	if (tok.type == TOK_PAREN_L) {
		sym.type.info.fun.ret_typ = malloc(sizeof(type_s));
		*sym.type.info.fun.ret_typ = *type;
		sym.type.type = TYPE_FUNC;
		sym.type.info.fun.params = vec_init(c_sym_s);
		parse_params(&sym.type.info.fun.params);
	} else {
		sym.type = *type;
	}
	return add_sym(&sym);
}

void gen_func(size_t sidx) {
	ast_node_s node;
	node.type = AST_FUNC;
	node.val.func.sym = sidx;
	node.val.func.body = malloc(sizeof(ast_node_s));
	init_scope(node.val.func.body);
	node.val.func.labels = vec_init(char *);
	node.val.func.body->val.scope.symbols = vec_dup(curr_scop->val.scope.symbols[sidx].type.info.fun.params); // a copy, not a transplant. may be suboptimal
	// TODO check if this function symbol already exists in the extern lists
	lexer_next();
	curr_scop = node.val.func.body;
	while (tok.type != TOK_BRACK_R) {
		handle_stmt();
	}
	curr_scop = node.val.func.body->val.scope.parent;

	add_node(&node);
}

void handle_decl(void) {
	type_s type;
	storag_e stor = STORE_IMPLICIT;
	memset(&type, 0, sizeof(type_s));

	while (tok_is_decl()) {
		if (tok_is_store()) {
			// TODO
		} else {
			mod_type(&type);
		}
	}

	if (tok.type == TOK_IDENT) {
		size_t sidx = gen_sym(&type, stor);
		if (tok.type == TOK_BRACK_L) {
			gen_func(sidx);
		}
	} else {
		error(ERR_SYNTAX, "Malformed declaration");
	}

	if (tok.type == TOK_COMMA) {
		do {
			lexer_next();
			gen_sym(&type, stor);
		} while (tok.type == TOK_COMMA);
	}
	// TODO make sure there's a semicolon
}
