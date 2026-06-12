#include "lexer.h"
#include "parse/ctx.h"
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

void lexer_next(struct parse_ctx *ctx) {
rewind:
	if (!ctx->lexer.skr) {
		ctx->lexer.tok.type = TK_NULL;
	}
	if (!*ctx->lexer.skr) {
		lexer_next_line(&ctx->lexer);
		goto rewind;
	}
	if (isspace(*ctx->lexer.skr)) {
		do {
			++ctx->lexer.skr;
		} while (isspace(*ctx->lexer.skr));
		goto rewind;
	}

	if (isdigit(*ctx->lexer.skr)) {
		ctx->lexer.tok.val = gen_constant(ctx);
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
