#ifndef __LEX__H__
#define __LEX__H__

#include "tok.h"
#include <stdio.h>

void lexer_init(FILE *file);
void lexer_next();
void lexer_free();

extern Token token;

#endif //!__LEX__H__
