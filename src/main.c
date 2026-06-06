#include "dict.h"
#include "lexer.h"
#include "tree.h"
#include <stdio.h>

int main(void) {
	puts("TinyC minimal C compiler");
	lxr_init(fopen("test/1.c", "r"));
	tree_parse();
	tree_print();
	return 0;
}
