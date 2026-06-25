#ifndef __LEXER__H__
#define __LEXER__H__

#define LINE_BUF_LEN 4096

#include <stdbool.h>
#include <stdio.h>

typedef enum {
#define TK(id, str) TOK_##id,
#include "token.h"
#undef TK
	TOK_VALUE,
	TOK_IDENT,
	TOK_EOF,
} tok_e;
typedef struct {
	tok_e type;
	size_t val;
} tok_s;

extern char const *const TOKENS[];

extern char line[LINE_BUF_LEN];
extern size_t line_num;
extern size_t col_num;
extern tok_s tok;

void lexer_open(FILE *file);
void lexer_next(void);
void lexer_close(void);

bool tok_is_kword(void);
bool tok_is_op_pref(void);
bool tok_is_op_suff(void);
bool tok_is_bin_op(void);
bool tok_is_punc(void);
bool tok_is_decl(void);
bool tok_is_op(void);

#endif //!__LEX__H__
