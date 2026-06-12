#include "parse.h"
#include "error.h"
#include "parse/ctx.h"
#include "parse/lex/toks.h"
#include "strucs/nodes.h"
#include "utils/hash.h"
#include "utils/pstr.h"
#include "utils/vector.h"

static void init_scope(struct ast_scope *scope, struct ast_scope *parent) {
	vec_init(scope->children);
	vec_init(scope->vars_line);
	scope->parent = parent;
	hashmap_init(&scope->vars_map);
}

static int gen_node(struct parse_ctx *ctx, struct ast_node *node) {
	return 1;
}

static void handle_expr(struct parse_ctx *ctx) {
	struct ast_node node;
	gen_node(ctx, &node);
}

static void handle_stmt(struct parse_ctx *ctx) {
	switch (ctx->lexer.tok.type) {
	case TK_NULL:
		parse_error(ctx);
	case TK_IDEN:
	case TK_PUNC:
	case TK_CNST: {
		handle_expr(ctx);
		break;
	}
	case TK_KEYW:
		break;
	}
}

void gen_tree(struct parse_ctx *ctx) {
	init_scope(&ctx->scope, NULL);
	ctx->current = &ctx->scope;
	while (ctx->lexer.tok.type != TK_NULL) {
		handle_stmt(ctx);
	}
}
