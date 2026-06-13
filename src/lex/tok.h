#ifndef __TOK__H__
#define __TOK__H__

#include "val.h"

enum {
	KW_INT,
	KW_CHAR,
	KW_SHORT,
	KW_LONG,

	KW_FLOAT,
	KW_DOUBLE,

	KW_AUTO,
	KW_STATIC,
	KW_EXTERN,
	KW_INLINE,

	KW_RETURN,
	KW_BREAK,
	KW_CONTINUE,
	KW_GOTO,

	KW_SIZEOF,
};

enum {
	PN_SEMI,
	PN_COLON,
	PN_COMMA,

	PN_PAREN_L,
	PN_PAREN_R,
	PN_BRACE_L,
	PN_BRACE_R,
	PN_BRACK_L,
	PN_BRACK_R,
};

typedef struct {
	enum {
		TOK_NONE,
		TOK_PUNC,
		TOK_KEYW,
		TOK_IDEN,
		TOK_CNST,
	} type;
	union {
		Const cnst;
		size_t indx;
		char *iden;
	};
} Token;

extern char const *const KEYWORDS[];
extern char const *const PUNCTUATORS[];

#endif //!__TOK__H__
