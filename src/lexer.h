#ifndef __LEXER__H__
#define __LEXER__H__

#include <stddef.h>
#include <stdio.h>

extern struct tok {
  enum toktyp { TokNone, TokConst, TokKword, TokIdent, TokPunc, TokOper } typ;
  size_t idx;
} tok;

extern struct cnst *cnsts;
extern size_t num_cnsts;

extern char **idens;
extern size_t num_idens;

void lxr_init(FILE *f);
void lxr_print(void);

void lxr_next_(void);
#define lxr_next() lxr_next_(), lxr_print()

#endif
