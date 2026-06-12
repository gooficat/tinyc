#include "lexer.h"
#include "strucs/value.h"
#include "toks.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void lexer_next_line(struct lexer *lexer);

void lexer_init(struct lexer *lexer, FILE *file) {
	lexer->file = file;
	lexer->line = malloc(lexer->line_cap = 128);
	lexer->line_num = 0;
	lexer_next_line(lexer);
}

void lexer_next(struct lexer *lexer) {
rewind:
	if (!lexer->skr) {
		lexer->tok.type = TK_NULL;
	}
	if (!*lexer->skr) {
		lexer_next_line(lexer);
		goto rewind;
	}
	if (isspace(*lexer->skr)) {
		do {
			++lexer->skr;
		} while (isspace(*lexer->skr));
		goto rewind;
	}

	if (isdigit(*lexer->skr)) {
		lexer->tok.val = gen_constant(&lexer->skr);
	}
}

void lexer_close(struct lexer *lexer) {
	fclose(lexer->file);
	free(lexer->line);
	memset(lexer, 0, sizeof(struct lexer));
}

static void lexer_next_line(struct lexer *lexer) {
	++lexer->line_num;
	lexer->skr = fgets(lexer->line, lexer->line_cap, lexer->file);
	if (!lexer->skr) {
		return;
	}
	while (lexer->line[strlen(lexer->line) - 1] != '\n') {
		size_t old_cap = lexer->line_cap;
		lexer->line = realloc(lexer->line, lexer->line_cap *= 2);
		if (!fgets(lexer->line + old_cap, old_cap, lexer->file)) {
			return;
		}
	}
}
