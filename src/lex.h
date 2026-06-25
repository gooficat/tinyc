#ifndef __LEX__H__
#define __LEX__H__

#define LINE_BUF_LEN 4096

#include <stdio.h>

typedef enum {
#define TK(id, str) id,
#include "token.h"
#undef TK
} tok_e;
typedef struct {
	tok_e type;
	size_t val;
} tok_s;

void lex_open(FILE *file);
void lex_next(void);
void lex_close(void);

#endif //!__LEX__H__
