#include "dict.h"
#include "lex.h"
#include <stdio.h>

int main(void) {
	Lexer lexer;
	Lexer_Init(&lexer, fopen("test/1.c", "rt"));
rptb:
	switch (lexer.token.type) {
	case TOK_NONE:
		printf("none\n");
		break;
	case TOK_CNST:
		printf("constant\n");
		goto rpt;
	case TOK_KEYW:
		printf("keyword %s\n", KEYWORDS[lexer.token.data.kw]);
		goto rpt;
	case TOK_IDEN:
		printf("identifier %s\n", lexer.token.data.id);
		goto rpt;
	case TOK_OPER:
		printf("operator %s\n", OPERATORS[lexer.token.data.op]);
		goto rpt;
	case TOK_PUNC:
		printf("punctuator %s\n", PUNCTUATORS[lexer.token.data.pn]);
		goto rpt;
	}
	return 0;
rpt:
	Lexer_Next(&lexer);
	goto rptb;
}
