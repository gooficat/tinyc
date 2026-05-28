#ifndef __LEX__H__
#define __LEX__H__

#include "dict.h"
#include "val.h"
#include <stdio.h>

typedef enum {
	TOK_NONE,
	TOK_PUNC,
	TOK_OPER,
	TOK_KEYW,
	TOK_IDEN,
	TOK_CNST
} TokenType;

typedef struct {
	struct {
		TokenType type;
		union {
			Operator   op;
			Punctuator pn;
			Keyword	   kw;
			char	  *id;
			CConst	   cn;
		} data;
	} token;
	FILE  *in_file;
	char  *skr, *line;
	size_t lml;
} Lexer;

void Lexer_Init(Lexer *lexer, FILE *in_file);
void Lexer_Next(Lexer *lexer);

#endif
