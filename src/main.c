#include "lexer.h"
#include <stdio.h>

int main(void) {
	puts("TinyC minimal C compiler");
	lxr_init(fopen("test/1.c", "r"));
	while (tok.typ) {
		lxr_next();
	}
}
