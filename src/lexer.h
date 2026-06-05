#ifndef __LEXER__H__
#define __LEXER__H__

#include <stddef.h>
#include <stdio.h>

extern struct tok {
  enum { TokNone, TokConst, TokKword, TokIdent, TokPunc, TokOper } typ;
  size_t idx;
} tok;

extern struct cnst *cnsts;
extern size_t num_cnsts;

extern char **idens;
extern size_t num_idens;

void lxr_init(FILE *f);
void lxr_next(void);

#endif
