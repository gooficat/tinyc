#include "lexer.h"
#include "error/error.h"
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
int col_num;
tok_s tok;

static void lexer_next_line(void) {
	if (!fgets(line, LINE_BUF_LEN, file)) {
		line[0] = '\0';
	} else if (line[strlen(line) - 1] != '\n') {
		error(ERR_SYNTAX, "Line length too long. Try a line below 4095 characters");
	}
	++line_num;
	col_num = 0;
}

void lexer_open(FILE *file_) {
	file = file_;
	line_num = 0;
	IDENTS = vec_init(char *);
	VALUES = vec_init(c_val_s);
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
	tok.type = TOK_VALUE;
	col_num = i;
}

static char *lexer_extract_word(void) {
	size_t i = 0;
	char *word;
	while (is_word_char(line[col_num + i])) {
		++i;
	}
	word = malloc(i + 1);
	memcpy(word, line + col_num, i);
	word[i] = '\0';
	col_num += i;
	return word;
}

static void lexer_handle_symbol(void) {
	tok.type = TOK_IDENT;

	for (tok.val = 0; tok.val < vec_len(IDENTS); ++tok.val) {
		size_t len = strlen(IDENTS[tok.val]);
		if (!memcmp(IDENTS[tok.val], line + col_num, len) &&
			(!is_word_char(line[col_num + len]) || !is_word_char(line[col_num + len - 1]))) {
			tok.val = tok.val;
			return;
		}
	}

	IDENTS = vec_grow(IDENTS, 1);
	IDENTS[tok.val] = lexer_extract_word();
}

void lexer_next(void) {
repeat:
	if (line[col_num] == '\0') {
		tok.type = TOK_EOF;
		goto finish;
	}
	while (isspace(line[col_num])) {
		if (line[col_num] == '\n') {
			lexer_next_line();
			goto repeat;
		}
		++col_num;
	}
	if (line[col_num] == '"') {
		lexer_handle_string();
		goto finish;
	}

	if (isdigit(line[col_num])) {
		lexer_handle_number();
		goto finish;
	}

	for (tok.type = 0; TOKENS[tok.type]; ++tok.type) {
		size_t len = strlen(TOKENS[tok.type]);
		if (!memcmp(line + col_num, TOKENS[tok.type], len) && (!is_word_char(line[col_num + len - 1]) || !is_word_char(line[col_num + len]))) {
			col_num += len;
			goto finish;
		}
	}

	lexer_handle_symbol();

finish:
	printf("current token ends at `%s`\n", line + col_num);
}

void lexer_close(void) {
	fclose(file);
}
