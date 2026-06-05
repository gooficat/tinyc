#include "dict.h"
#include "lexer.h"
#include "val.h"
#include <stdio.h>

int main(void) {
	puts("TinyC minimal C compiler");
	lxr_init(fopen("test/1.c", "r"));
rpt:
	switch (tok.typ) {
	case TokNone:
		puts("End of stream");
		goto done;
	case TokConst:
		printf("Constant of type %i\n", cnsts[tok.idx].typ);
		break;
	case TokKword:
		printf("Keyword %s\n", KEYWORDS[tok.idx]);
		break;
	case TokIdent:
		printf("Identifier %s\n", idens[tok.idx]);
		break;
	case TokPunc:
		printf("Punctuator %c\n", PUNCTUATORS[tok.idx]);
		break;
	case TokOper:
		printf("Operator %s\n", OPERATORS[tok.idx]);
		break;
	}
	lxr_next();
	goto rpt;
done:
	return 0;
}
