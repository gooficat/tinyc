#include "tiny.h"
#include "lexer.h"
#include <stdio.h>

void print_tok(void) {
	if (tok.type == TOK_IDENT) {
		printf("IDENT `%s`\n", IDENTS[tok.val]);
	} else if (tok.type == TOK_VALUE) {
		printf("VALUE\n");
	} else {
		printf("TOK %s\n", TOKENS[tok.type]);
	}
	lexer_next();
}

int main(void) {
	lexer_open(fopen("./test/1.c", "r"));
	parse_tree();

	return 0;
}
