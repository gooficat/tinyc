#include "dict.h"
#include "lexer.h"
#include "val.h"
#include <stdio.h>

int main(void) {
	puts("TinyC minimal C compiler");
	lxr_init(fopen("test/1.c", "r"));
	while (tok.typ != TokNone) {
		lxr_next();
	}
	return 0;
}
