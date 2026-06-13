#include "lex.h"
#include "tok.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_LINE_MAX 128

#define valid_word_char(c) ((c) == '_' || isalnum(c))

static FILE *file;
static char *line;
static char *skr;
static size_t line_max;
Token token;

void lexer_init(FILE *file_) {
	file = file_;
	skr = line = malloc(line_max = INITIAL_LINE_MAX);
}

static void lexer_next_line() {
	if (!(skr = fgets(line, line_max, file)))
		return;
	while (line[strlen(line) - 1] != '\n') {
		size_t old_cap = line_max;
		line = realloc(line, line_max *= 2);

		if (!fgets(line + old_cap, old_cap, file))
			break;
	}
	skr = line;
}

static void lexer_trim() {
	while (skr)
		if (!*skr)
			lexer_next_line();
		else if (isspace(*skr))
			++skr;
		else
			return;
}

void lexer_next() {
	lexer_trim();

	if (!skr) {
		token.type = TOK_NONE;
		return;
	}

	if (*skr == '"') {
		token.type = TOK_CNST;
		++skr;
		size_t i = 0;
		while (skr[i - 1] == '\\' || skr[i] != '"')
			++i;

		token.cnst.type = CONST_STR;
		token.cnst.s = malloc(i + 1);
		memcpy(token.cnst.s, skr, i);
		token.cnst.s[i] = '\0';
		skr += i + 1;
	} else if (isdigit(*skr)) {
		token.type = TOK_CNST;

		size_t i = 1;
		while (isalnum(skr[i]))
			++i;

		if (skr[i] == '.') {
			token.cnst.type = CONST_FLT;
			token.cnst.f = strtold(skr, &skr);
		} else {
			token.cnst.type = CONST_INT;
			token.cnst.f = strtoll(skr, &skr, 0);
		}
	} else {
		for (char const *const *puncs = PUNCTUATORS; *puncs; ++puncs) {
			size_t len = strlen(*puncs);
			if (!memcmp(*puncs, skr, len)) {
				token.type = TOK_PUNC;
				token.indx = puncs - PUNCTUATORS;

				skr += len;
				return;
			}
		}

		for (char const *const *kwords = PUNCTUATORS; *kwords; ++kwords) {
			size_t len = strlen(*kwords);
			if (!memcmp(*kwords, skr, len) && //
				!valid_word_char(skr[len])) {

				token.type = TOK_KEYW;
				token.indx = kwords - KEYWORDS;

				skr += len;
				return;
			}
		}

		token.type = TOK_IDEN;

		size_t i = 0;
		while (valid_word_char(skr[i]))
			++i;

		token.iden = malloc(i + 1);
		memcpy(token.iden, skr, i);
		token.iden[i] = '\0';
	}
}

void lexer_free() {
	fclose(file);
	file = NULL;
	free(line);
	line = NULL;
	skr = NULL;
	line_max = 0;
}
