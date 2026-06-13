#include "lex.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_LINE_MAX 128

static FILE *file;
static char *line;
static char *skr;
static size_t line_max;

void lexer_init(FILE *file_) {
	file = file_;
	skr = line = malloc(line_max = INITIAL_LINE_MAX);
}

void lexer_next() {
}

void lexer_free() {
	fclose(file);
	file = NULL;
	free(line);
	line = NULL;
	skr = NULL;
	line_max = 0;
}
