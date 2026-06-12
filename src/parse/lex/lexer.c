#include "lexer.h"
#include "parse/ctx.h"
#include "strucs/value.h"
#include "toks.h"
#include "utils/hash.h"
#include "utils/pstr.h"
#include "utils/vector.h"
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

char const *const *word_match(char const *src, char const *const *list) {
	while (*list) {
		size_t len = strlen(*list);
		if (strncmp(src, *list, len)) {
			++list;
			continue;
		}
		if (isalnum(src[len - 1]) && isalnum(src[len])) {
			continue;
		}
		return list;
	}
	return NULL;
}

void lexer_next(struct parse_ctx *ctx) {
rewind:
	if (!ctx->lexer.skr) {
		ctx->lexer.tok.type = TK_NULL;
		return;
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

	if (*ctx->lexer.skr == '"' || isdigit(*ctx->lexer.skr)) {
		ctx->lexer.tok.type = TK_CNST;
		ctx->lexer.tok.val = gen_constant(ctx);
		return;
	}
	{
		char const *const *match;
		match = word_match(ctx->lexer.skr, PUNCTUATORS_UNMAPPED);
		if (match) {
			ctx->lexer.tok.type = TK_PUNC;
			ctx->lexer.tok.val = match - PUNCTUATORS_UNMAPPED;
			ctx->lexer.skr += strlen(PUNCTUATORS_UNMAPPED[ctx->lexer.tok.val]);
			return;
		}
		match = word_match(ctx->lexer.skr, KEYWORDS_UNMAPPED);
		if (match) {
			ctx->lexer.tok.type = TK_KEYW;
			ctx->lexer.tok.val = match - KEYWORDS_UNMAPPED;
			ctx->lexer.skr += strlen(KEYWORDS_UNMAPPED[ctx->lexer.tok.val]);
			return;
		}
	}
	{
		size_t i = 0, idx = vec_len(ctx->identifiers);
		while (isalnum(ctx->lexer.skr[++i]))
			;
		vec_grow(ctx->identifiers, 1);
		pstr_dup(&ctx->identifiers[idx], ctx->lexer.skr, 0, i);
		ctx->lexer.skr += i;
		ctx->lexer.tok.type = TK_IDEN;
		ctx->lexer.tok.val = idx;
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
