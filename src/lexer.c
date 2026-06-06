#include "lexer.h"
#include "dict.h"
#include "val.h"
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *in;
static char *line, *skr;
static size_t line_cap;
struct tok tok;

struct cnst *cnsts;
size_t num_cnsts = 0;

char **idens;
size_t num_idens = 0;

static void lxr_getline(void) {
	skr = fgets(line, line_cap, in);
	while (skr && line[strlen(line) - 1] != '\n') {
		size_t old = line_cap;
		line = realloc(line, line_cap *= 2);
		if (!fgets(line + old, old, in)) {
			return;
		}
	}
}

void lxr_init(FILE *f) {
	in = f;
	line_cap = 128;
	line = malloc(line_cap);
	cnsts = malloc(1);
	idens = malloc(1);
	lxr_getline();
	lxr_next();
}

void lxr_print(void) {
	printf("Token:\n\t");
	switch (tok.typ) {
	case TokNone:
		puts("End of stream");
		break;
	case TokConst:
		printf("Constant of type %i\n", cnsts[tok.idx].typ);
		break;
	case TokKword:
		printf("Keyword %s\n", KEYWORDS[tok.idx]);
		break;
	case TokIdent:
		printf("Identifier %s\n", idens[tok.idx]);
		break;
	case TokPunc:
		printf("Punctuator %c\n", PUNCTUATORS[tok.idx]);
		break;
	case TokOper:
		printf("Operator %s\n", OPERATORS[tok.idx]);
		break;
	}
	fflush(stdout);
}

void lxr_next_(void) {
	size_t j;
rpt:
	if (!skr) {
		tok.typ = TokNone;
		puts("Finished reading from file");
		fflush(stdout);
		fclose(in);
		if (line) {
			free(line);
			line = NULL;
		}
		return;
	}
	if (!*skr) {
		lxr_getline();
		goto rpt;
	}
	if (*skr == '"') {
		tok.typ = TokConst;
		++skr;
		j = 0;
		while (skr[j] != '"' || skr[j] == '\\') {
			++j;
		}
		for (tok.idx = 0; tok.idx < num_cnsts; ++tok.idx) {
			if (cnsts[tok.idx].typ == ConstString || j > strlen(cnsts[tok.idx].val.s)) {
				continue;
			}
			if (!strncmp(skr, cnsts[tok.idx].val.s, j)) {
				return;
			}
		}
		cnsts = realloc(cnsts, ++num_cnsts * sizeof(*cnsts));
		cnsts[tok.idx].typ = ConstString;
		cnsts[tok.idx].val.s = malloc(j + 1);
		strncpy((char *)cnsts[tok.idx].val.s, skr, j);
		skr += j;
		return;
	}
	if (isspace(*skr)) {
		++skr;
		goto rpt;
	}
	if (isdigit(*skr)) {
		tok.typ = TokConst;
		tok.idx = num_cnsts;
		cnsts = realloc(cnsts, ++num_cnsts * sizeof *cnsts);
		cnsts[tok.idx].typ = ConstInt;
		cnsts[tok.idx].val.i = strtol(skr, &skr, 0);
		return;
	}
	for (tok.idx = 0; KEYWORDS[tok.idx]; ++tok.idx) {
		j = strlen(KEYWORDS[tok.idx]);
		if (!strncmp(KEYWORDS[tok.idx], skr, j)) {
			if ((skr[j] != '_' && !isalnum(skr[j]))) {
				tok.typ = TokKword;
				skr += j;
				return;
			}
		}
	}
	for (tok.idx = 0; PUNCTUATORS[tok.idx]; ++tok.idx) {
		if (PUNCTUATORS[tok.idx] == *skr) {
			tok.typ = TokPunc;
			++skr;
			return;
		}
	}
	for (tok.idx = 0; OPERATORS[tok.idx]; ++tok.idx) {
		j = strlen(OPERATORS[tok.idx]);
		if (!strncmp(OPERATORS[tok.idx], skr, j)) {
			tok.typ = TokOper;
			skr += j;
			return;
		}
	}
	tok.typ = TokIdent;
	j = 0;
	while (skr[j] == '_' || isalnum(skr[j])) {
		++j;
	}
	for (tok.idx = 0; tok.idx < num_idens; ++tok.idx) {
		if (j > strlen(idens[tok.idx])) {
			continue;
		}

		if (!strncmp(idens[tok.idx], skr, j)) {
			skr += j;
			return;
		}
	}

	idens = realloc(idens, ++num_idens * sizeof(*idens));
	idens[tok.idx] = malloc(j + 1);
	memcpy(idens[tok.idx], skr, j);
	idens[tok.idx][j] = 0;
	skr += j;
}
