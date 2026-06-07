#include "dict.h"
#include "gen.h"
#include "lexer.h"
#include "tree.h"
#include <stdio.h>

int main(void) {
	puts("TinyC minimal C compiler");
	lxr_init(fopen("test/2.c", "r"));
	tree_parse();
	tree_print();
	codegen_init(fopen("bin/2.s", "w"));
	codegen_tree();
	return 0;
}
