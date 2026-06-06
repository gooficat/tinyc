#include "err.h"
#include "lexer.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void parse_panic(char const *msg) {
	struct tok *tokk = &tok;
	fflush(stdout);
	fputs(msg, stderr);
	exit(EXIT_FAILURE);
}
