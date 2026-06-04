#ifndef __LEXER__H__
#define __LEXER__H__

#include <stddef.h>
#include <stdio.h>

extern struct tok {
  enum { TokNone, TokConst, TokKword, TokIdent } typ;
  size_t idx;
} tok;

void lxr_init(FILE *f);
void lxr_next(void);

#endif
