#include "lexer.h"
#include "tokns.h"
#include <stdio.h>

int main()
{
	tok_strm_s ts;
	puts("Hello");
	tok_strm__init(&ts, "test/1.c");
	while (ts.tok.typ != TOK_EOF)
	{
		switch (ts.tok.typ)
		{
		case TOK_KEYW:
			printf("keyword %s\n", KEYWORDS[ts.tok.num]);
			break;
		case TOK_PUNC:
			printf("punctuator %s\n", PUNCTUATORS[ts.tok.num]);
			break;
		case TOK_OPER:
			printf("operator %s\n", OPERATORS[ts.tok.num]);
			break;
		case TOK_IDENT:
			printf("identifier %s\n", ts.ident_pool.data[ts.tok.num]);
			break;
		case TOK_STRLIT:
			printf("string \"%s\"\n", ts.str_pool.data[ts.tok.num]);
			break;
		case TOK_CONST:
			printf("constant %lld\n", ts.tok.num);
		case TOK_EOF:
			break; // unreachable
		}
		tok_strm__next(&ts);
	}
	return 0;
}
