#include "lex.h"
#include "dict.h"
#include "val.h"
#include <ctype.h>
#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Lexer_ReadLine(Lexer *lexer);

void Lexer_Init(Lexer *lexer, FILE *in_file) {
	lexer->in_file = in_file;
	lexer->line	   = malloc(512);
	lexer->lml	   = 512;
	Lexer_ReadLine(lexer);
	Lexer_Next(lexer);
}

static void Lexer_ReadLine(Lexer *lexer) {
	size_t ll;
	lexer->skr = fgets(lexer->line, lexer->lml, lexer->in_file);
	ll		   = strlen(lexer->line);
	while (lexer->line[ll - 1] != '\n') {
		lexer->line = realloc(lexer->line, (lexer->lml *= 2));
		lexer->skr	= fgets(lexer->line + ll, lexer->lml - ll, lexer->in_file);
	}
}

static int Lexer_SearchDict(char *const src, char const *const *dict) {
	size_t i;
	size_t l = strlen(src);
	for (i = 0; dict[i]; ++i) {
		size_t m = strlen(dict[i]);
		if (m > l) {
			continue;
		}
		if (strncmp(src, dict[i], m)) {
			continue;
		}
		if (m == l) {
			return i;
		}
		if (isalnum(src[l]) && isalnum(src[l - 1])) {
			continue;
		}
		return i;
	}
	return -1;
}

void Lexer_Next(Lexer *lexer) {
	long i;
recheck:
	if (!lexer->skr) {
		lexer->token.type = TOK_NONE;
		return;
	}
	if (!*lexer->skr) {
		Lexer_ReadLine(lexer);
		goto recheck;
	}
	if (isspace(*lexer->skr)) {
		++lexer->skr;
		goto recheck;
	}

	if (*lexer->skr == '"') {
		lexer->token.type		  = TOK_CNST;
		lexer->token.data.cn.type = CCONST_STRING;
		++lexer->skr;
		i = 0;
		do {
			i = (strchr(lexer->skr + i, '"') - lexer->skr);
		} while (lexer->skr[i - 1] != '\\');
		lexer->token.data.cn.val.s = malloc(i + 1);
		strncpy((char *)lexer->token.data.cn.val.s, lexer->skr, i - 1);
		lexer->skr += i;
		lexer->skr += i;
		return;
	}

	if (isdigit(*lexer->skr)) {
		lexer->token.type = TOK_CNST;
		for (i = 0; isalnum(lexer->skr[i]); ++i)
			;
		if (lexer->skr[i] == '.') {
			while (isdigit(lexer->skr[++i]))
				;
			lexer->token.data.cn.type  = CCONST_FLOAT;
			lexer->token.data.cn.val.f = strtod(lexer->skr, NULL);
		} else {
			lexer->token.data.cn.type  = CCONST_INTEG;
			lexer->token.data.cn.val.i = strtoll(lexer->skr, NULL, 0);
		}
		lexer->skr += i;
		return;
	}

	if (*lexer->skr == '_' || isalpha(*lexer->skr)) {
		i = Lexer_SearchDict(lexer->skr, KEYWORDS);
		if (i != -1) {
			lexer->token.type	 = TOK_KEYW;
			lexer->token.data.kw = i;
			lexer->skr += strlen(KEYWORDS[i]);
			return;
		}
		for (i = 0; lexer->skr[i] == '_' || isalpha(lexer->skr[i]); ++i)
			;
		lexer->token.type	 = TOK_IDEN;
		lexer->token.data.id = malloc(i + 1);
		memcpy(lexer->token.data.id, lexer->skr, i);
		lexer->token.data.id[i] = 0;
		lexer->skr += i;
		return;
	}

	i = Lexer_SearchDict(lexer->skr, PUNCTUATORS);
	if (i != -1) {
		lexer->token.type	 = TOK_PUNC;
		lexer->token.data.pn = i;
		lexer->skr += strlen(PUNCTUATORS[i]);
		return;
	}

	i = Lexer_SearchDict(lexer->skr, OPERATORS);
	if (i != -1) {
		lexer->token.type	 = TOK_OPER;
		lexer->token.data.pn = i;
		lexer->skr += strlen(OPERATORS[i]);
		return;
	}
}
