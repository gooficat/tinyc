#ifndef __LEXER__H__
#define __LEXER__H__

#include "parse/lex/toks.h"
#include "utils/pstr.h"
#include "utils/vector.h"
#include <stdint.h>
#include <stdio.h>

struct lexer {
	struct tok tok;
	char *line;
	size_t line_cap;
	char *skr;
	size_t line_num;
	FILE *file;
};

void lexer_init(struct lexer *lexer, FILE *file);
void lexer_next(struct lexer *lexer);
void lexer_close(struct lexer *lexer);

#endif //!__LEXER__H__
