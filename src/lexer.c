#include "lexer.h"
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

static FILE *in;
static char *line, *skr;
static size_t line_cap = 128;
struct tok tok;

static void lxr_getline(void) {
	skr = fgets(line, line_cap, in);
	while (skr && line[strlen(line) - 1] != '\n') {
		size_t old = line_cap;
		line = (char *)realloc(line, line_cap *= 2);
		if (!fgets(line + old, old, in)) {
			return;
		}
	}
}

void lxr_init(FILE *f) {
	in = f;
	line = malloc(line_cap);
	lxr_getline();
	lxr_next();
}

void lxr_next(void) {
rpt:
	if (!skr) {
		tok.typ = TokNone;
		puts("Finished reading from file");
		fclose(in);
		free(line);
		return;
	}
	if (!*skr) {
		lxr_getline();
		goto rpt;
	}
	if (isspace(*skr)) {
		++skr;
		goto rpt;
	}
	tok.typ = TokIdent;
	printf("line '%c'\n", *skr++);
}
