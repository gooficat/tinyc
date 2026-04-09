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
} tok_typ_e;

typedef struct
{
	tok_typ_e typ;
	union
	{
		long long num;
		double	  flt;
		char	 *str;
	};
} tok_s;

typedef struct
{
	tok_s		tok;
	char	   *buf;
	char	   *skr;
	vec_pchar_s str_pool;
	vec_pchar_s ident_pool;
} tok_strm_s;

void tok_strm__init(tok_strm_s *ts, const char *pat);
void tok_strm__next(tok_strm_s *ts);
void tok_strm__destroy(tok_strm_s *ts);

#endif //!__LEXER__H__
