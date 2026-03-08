#include "lex.h"
#include "tok.h"

void lexerinit(lexer *l, const char *fpath)
{
	tokstrminit(&l->ts, fpath);
}
