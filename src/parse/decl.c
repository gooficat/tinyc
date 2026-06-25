#include "lexer.h"
#include "tiny.h"
#include "type.h"
#include <string.h>

void mod_type(type_s *typ) {
	switch ((int)tok.type) {
	case TOK_KW_INT:
		if (typ->type == TYPE_NONE) {
			typ->type = TYPE_INT;
		} else {
		}
		// if (typ->info.igr.type == )
		typ->info.igr.type = INT_PLAIN;
		break;
	case TOK_KW_CHAR:
	case TOK_KW_SHORT:
	case TOK_KW_LONG:
	case TOK_KW_FLOAT:
	case TOK_KW_DOUBLE:
	case TOK_KW_UNSIGNED:
	case TOK_KW_SIGNED:
	case TOK_KW_STRUCT:
	case TOK_KW_UNION:
	case TOK_KW_ENUM:
	}
}

void handle_decl(void) {
	c_sym_s sym;
	memset(&sym, 0, sizeof(c_sym_s));
}
