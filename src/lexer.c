#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *file;

char line[LINE_BUF_LEN];
size_t line_num;
size_t col_num;
tok_s tok;

static void lexer_next_line(void) {
	if (!fgets(line, LINE_BUF_LEN, file)) {
		line[0] = '\0';
	}
	if (line[strlen(line) - 1] != '\n') {
		// TODO error for too long lines
	}
	++line_num;
	col_num = 0;
}

void lexer_open(FILE *file_) {
	file = file_;
	lexer_next_line();
	lexer_next();
}

void lexer_next(void) {
repeat:
	if (line[col_num] == '\0') {
		tok.type = TOK_EOF;
		return;
	}
	while (isspace(line[col_num])) {
		++col_num;
	}
	if (line[col_num] == '\n') {
		lexer_next_line();
		goto repeat;
	}

	//
}

void lexer_close(void) {
	//
}
