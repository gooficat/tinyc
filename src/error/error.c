#include "error.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

static char const *const ERROR_TYPES[] = {
	"INTERNAL",
	"FILE",
	"SYNTACTICAL",
	"CODE STRUCTURE",
	"CODE GENERATION",
};

void error_print_context(void) {
	fputs(line, stdout);
	printf("%*c^\n", (int)col_num, ' ');
}

void error(error_e type, char const *msg) {
	printf("ERROR: %s ERROR\n`%s`\n", ERROR_TYPES[type], msg);
	if (type == ERR_SYNTAX) {
		error_print_context();
	}
	exit(EXIT_FAILURE);
}
