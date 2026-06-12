#include "error.h"
#include "parse/ctx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void parse_error(struct parse_ctx *ctx) {
	fprintf(stderr, "Parse error\n");
	exit(EXIT_FAILURE);
}

noreturn void syntax_error(struct parse_ctx *ctx) {
	fprintf(stderr, "Syntax error\n");
	exit(EXIT_FAILURE);
}

noreturn void internal_error() {
	fprintf(stderr, "Internal error\n");
	exit(EXIT_FAILURE);
}
