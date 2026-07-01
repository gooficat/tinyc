#include "tiny.h"
#include "lexer.h"
#include <stdio.h>

int main(void) {
	lexer_open(fopen("./test/1.c", "r"));
	while (tok.type != TOK_EOF) {
		if (tok.type == TOK_IDENT) {
			printf("IDENT `%s`\n", IDENTS[tok.val]);
		} else if (tok.type == TOK_VALUE) {
			printf("VALUE\n");
		} else {
			printf("TOK %s\n", TOKENS[tok.type]);
		}
		lexer_next();
	}
	puts("EOF");
	return 0;
}
