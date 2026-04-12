#include "lexer.h"
#include "nodes.h"
#include <stdio.h>

extern void ast_gen(ast_scope *sc, tok_strm *ts);

int main()
{
	tok_strm  ts;
	ast_scope sc;
	puts("Hello");
	tok_strm__init(&ts, "test/1.c");
	ast_gen(&sc, &ts);
	tok_strm__destroy(&ts);
	return 0;
}
