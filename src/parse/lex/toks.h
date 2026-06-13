#ifndef __TOKS__H__
#define __TOKS__H__

#include "utils/vector.h"
enum tok_type {
	TK_NULL,
	TK_IDEN,
	TK_PUNC,
	TK_CNST,
	TK_KEYW,
};

enum keyword {
	KW_SIGNED,
	KW_UNSIGNED,
	KW_SHORT,
	KW_LONG,
	KW_CHAR,

	KW_STRUCT,
	KW_UNION,

	KW_RETURN,
	KW_GOTO,

	KW_IF,

	KW_SIZEOF,
};

enum punctuator {
	PN_PAREN_L,
	PN_PAREN_R,
	PN_BRACE_L,
	PN_BRACE_R,
	PN_BRACK_L,
	PN_BRACK_R,

	PN_SEMI,
	PN_COLON,
	PN_COMMA,
};

struct tok {
	enum tok_type type;
	size_t val;
};

extern char const *KEYWORDS_UNMAPPED[];
extern char const *PUNCTUATORS_UNMAPPED[];

extern const struct hash_map KEYWORDS;
extern const struct hash_map PUNCTUATORS;

void init_tok_maps();

bool tok_match(struct tok *tok, enum tok_type type, size_t val);

#endif //!__TOKS__H__
