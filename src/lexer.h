#ifndef __LEXER__H__
#define __LEXER__H__

#include "vectr.h"

typedef char *pchar;
vec_type(pchar);

typedef enum
{
	TOK_EOF,
	TOK_PUNC,
	TOK_KEYW,
	TOK_OPER,
	TOK_CONST,
	TOK_STRLIT,
	TOK_IDENT,
} tok_type;

typedef struct
{
	tok_type typ;
	union
	{
		long long num;
		double	  flt;
		char	 *str;
	};
} tok;

typedef struct
{
	tok		  tok;
	char	 *buf;
	char	 *skr;
	vec_pchar str_pool;
	vec_pchar ident_pool;
} tok_strm;

void tok_strm__init(tok_strm *ts, const char *pat);
void tok_strm__next(tok_strm *ts);
void tok_strm__destroy(tok_strm *ts);

#endif //!__LEXER__H__
