#include "err.h"
#include "lexer.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void parse_panic(char const *msg) {
	fflush(stdout);
	fputs(msg, stderr);
	exit(EXIT_FAILURE);
}

void codegen_panic(char const *msg) {
	fflush(stdout);
	fputs(msg, stderr);
	exit(EXIT_FAILURE);
}
