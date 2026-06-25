#include "lexer.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include <ctype.h>
#include <inttypes.h>
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
	line_num = 0;
	lexer_next_line();
	lexer_next();
}

static bool is_word_char(char c) {
	return c == '_' || isalnum(c);
}

static void lexer_handle_string(void) {
	size_t i = 0;
	char *str;
	tok.type = TOK_VALUE;
	++col_num;
	while (line[col_num + i - 1] == '\\' || line[col_num + i] != '"') {
		++i;
	}

	for (size_t j = 0; j < vec_len(VALUES); ++j) {
		if (VALUES[j].type == C_VAL_STRING && !strncmp(VALUES[j].val.str, line + col_num, i)) {
			tok.val = j;
			return;
		}
	}

	str = malloc(i + 1);
	memcpy(str, line + col_num, i);
	str[col_num] = '\0';
	col_num += i + 1;

	i = vec_len(VALUES);
	VALUES = vec_grow(VALUES, 1);
	VALUES[i].type = C_VAL_STRING;
	VALUES[i].val.str = str;
}

static void lexer_handle_number(void) {
	c_val_s c_val;
	size_t i = col_num;
	while (isdigit(line[i])) {
		++i;
	}
	if (line[i] == '.') {
		c_val.type = C_VAL_FLOAT;
		c_val.val.flt = strtold(line + col_num, NULL);
		do {
			++i;
		} while (isdigit(line[i]));
	} else {
		c_val.type = C_VAL_INT;
		c_val.val.igr = strtoimax(line + col_num, NULL, 0);
	}
	col_num = i;
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

	if (line[col_num] == '"') {
		lexer_handle_string();
	}

	if (isdigit(line[col_num])) {
		lexer_handle_number();
	}

	for (tok.type = 0; TOKENS[tok.type]; ++tok.type) {
	}
}

void lexer_close(void) {
	//
}
