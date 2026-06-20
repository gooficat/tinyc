#ifndef __LEX__H__
#define __LEX__H__

#include "tok.h"
#include <stdbool.h>
#include <stdio.h>


void lexer_init(FILE *file);
void lexer_next();
void lexer_free();

bool tok_is(int type, size_t idx);

bool kw_is_type();
bool kw_is_order();
bool kw_is_storage();

extern Token token;

#endif //!__LEX__H__
