#include "lexer.h"

char const *const TOKENS[] = {
#define TK(id, str) str,
#include "token.h"
#undef TK
};

bool tok_is_kword(void) {
	return tok.type >= TOK_KW_INT && tok.type <= TOK_KW_NORETURN;
}
bool tok_is_op_pref(void) {
	switch ((int)tok.type) {
	case TOK_OP_ADD:
	case TOK_OP_SUB:
	case TOK_OP_MUL:
	case TOK_OP_INC:
	case TOK_OP_DEC:
	case TOK_OP_BIN_NOT:
	case TOK_OP_LOG_NOT:
	case TOK_OP_BIN_AND:
		return true;
	default:
		return false;
	}
}
bool tok_is_op_suff(void) {
	switch ((int)tok.type) {
	case TOK_OP_INC:
	case TOK_OP_DEC:
		return true;
	default:
		return false;
	}
}
bool tok_is_bin_op(void) {
	return tok.type >= TOK_OP_ADD && tok.type <= TOK_OP_ASS;
}
bool tok_is_punc(void) {
	return tok.type >= TOK_PAREN_L && tok.type <= TOK_COMMA;
}
bool tok_is_decl(void) {
	return tok.type >= TOK_KW_INT && tok.type <= TOK_KW_NORETURN;
}
bool tok_is_op(void) {
	// TECHNICALLY ignores *those* operators, dot, arrow and array. but those are really more like punctuators
	return tok.type >= TOK_OP_INC && tok.type <= TOK_OP_ASS;
}
