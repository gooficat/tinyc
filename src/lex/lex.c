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

static void lex_str() {
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
}

static void lex_num() {
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
}

static int lex_cnst() {
	if (*skr == '"') {
		lex_str();
	} else if (isdigit(*skr)) {
		lex_num();
	} else {
		return 0;
	}
	return 1;
}

static int lex_kword() {
	for (char const *const *puncs = PUNCTUATORS; *puncs; ++puncs) {
		size_t len = strlen(*puncs);
		if (!memcmp(*puncs, skr, len)) {
			token.indx = puncs - PUNCTUATORS;
			skr += len;
			return 1;
		}
	}
	return 0;
}

static int lex_punc() {
	for (char const *const *kwords = PUNCTUATORS; *kwords; ++kwords) {
		size_t len = strlen(*kwords);
		if (!memcmp(*kwords, skr, len) && //
			!valid_word_char(skr[len])) {
			token.indx = kwords - KEYWORDS;
			skr += len;
			return 1;
		}
	}
	return 0;
}

static void lex_word() {
	token.type = TOK_IDEN;

	size_t i = 0;
	while (valid_word_char(skr[i]))
		++i;

	token.iden = malloc(i + 1);
	memcpy(token.iden, skr, i);
	token.iden[i] = '\0';
}

void lexer_next() {
	lexer_trim();

	if (!skr) {
		token.type = TOK_NONE;
	} else if (lex_cnst()) {
		token.type = TOK_CNST;
	} else if (lex_punc()) {
		token.type = TOK_PUNC;
	} else if (lex_kword()) {
		token.type = TOK_KEYW;
	} else {
		lex_word();
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

bool tok_is(int type, size_t idx) {
	return token.type == type && token.indx == idx;
}

bool kw_is_type() {
	return token.indx <= KW_INLINE;
}

bool kw_is_order() {
	return token.indx >= KW_RETURN && token.indx <= KW_GOTO;
}

bool kw_is_storage() {
	return kw_is_order() && token.indx >= KW_AUTO;
}
