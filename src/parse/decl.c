#include "error/error.h"
#include "lexer.h"
#include "tiny.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

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

static vec(type_s) parse_params(void) {
	// TODO it needs to be a type-name pair without storage specification
	vec(type_s) params = vec_init(type_s);
}

size_t add_sym(type_s *type, storag_e stor) {
	c_sym_s sym;
	sym.storage = stor;
	sym.name = tok.val;
	lexer_next();
	if (tok.type == TOK_PAREN_L) {
		sym.type.info.fun.ret_typ = malloc(sizeof(type_s));
		*sym.type.info.fun.ret_typ = *type;
		sym.type.type = TYPE_FUNC;
		sym.type.info.fun.params = parse_params();
		lexer_next();
	} else {
		sym.type = *type;
	}
}

void handle_decl(void) {
	type_s type;
	storag_e stor = STORE_IMPLICIT;
	memset(&type, 0, sizeof(type_s));
	if (tok_is_store()) {
		// TODO
	}
	if (tok_is_tag()) {
	} else {
		while (tok_is_decl()) {
			mod_type(&type);
		}
	}
	if (tok_is_store()) {
		if (stor != STORE_IMPLICIT) {
			error(ERR_SYNTAX, "Specifying storage more than once");
		}
		// TODO
	}
	if (tok.type == TOK_IDENT) {
		add_sym(&type, stor);
	} else {
		error(ERR_SYNTAX, "Malformed declaration");
	}
	if (tok.type == TOK_COMMA) {
		do {
			lexer_next();
			add_sym(&type, stor);
		} while (tok.type == TOK_COMMA);
	}
	// TODO make sure there's a semicolon
}
