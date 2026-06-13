#ifndef __TOK__H__
#define __TOK__H__

#include "val.h"

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
	};
} Token;

#endif //!__TOK__H__
