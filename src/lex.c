#include "lex.h"
#include "da.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

void Lexer_Init(Lexer *lexer, FILE *in_file) {
}

static void Lexer_ReadLine(Lexer *lexer) {
}

static void Lexer_Advance(Lexer *lexer, size_t n) {
rpt:
	while (*lexer->skr) {
		++lexer->skr;
		--n;
	}
	if (n) {
		Lexer_ReadLine(lexer);
		goto rpt;
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
	while (isspace(*lexer->skr)) {
	}
}
