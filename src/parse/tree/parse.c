#include "parse.h"
#include "error.h"
#include "parse/ctx.h"
#include "parse/lex/lexer.h"
#include "parse/lex/toks.h"
#include "strucs/nodes.h"
#include "strucs/symbol.h"
#include "utils/hash.h"
#include "utils/vector.h"

static void init_scope(struct ast_scope *scope, struct ast_scope *parent) {
	scope->children = vec_init(struct ast_node);
	scope->vars_line = vec_init(struct c_var);
	scope->parent = parent;
	hashmap_init(&scope->vars_map);
}

static int gen_node(struct parse_ctx *ctx, struct ast_node *node) {
	switch (ctx->lexer.tok.type) {
	case TK_PUNC:
	case TK_CNST:
	case TK_IDEN:
	case TK_KEYW:
		/*ALWAYS a cast, this ONLY happens when we get a parentheses recursed drop-down*/
		node->type = AST_CAST;
		break;
	default:
		internal_error();
	}

	if (ctx->lexer.tok.type == TK_PUNC) {
		if (ctx->lexer.tok.val == PN_PAREN_L) {
			lexer_next(&ctx->lexer);
		}
	}
	return 1;
}

static void handle_expr(struct parse_ctx *ctx) {
	struct ast_node node;
	if (!gen_node(ctx, &node)) {
		parse_error(ctx);
	}
	if (ctx->lexer.tok.val == PN_COMMA) { /*chained statements.*/
		struct ast_node *chain = vec_init(struct ast_node);
		vec_grow(chain, 1);
		chain[0] = node;
		do {
			size_t i = vec_len(chain);
			lexer_next(&ctx->lexer);
			vec_grow(chain, 1);
			gen_node(ctx, &chain[i]);
		} while (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_COMMA);
		node.type = AST_CHAIN;
		node.val = chain;
	}
}

static void handle_stmt(struct parse_ctx *ctx) {
	switch (ctx->lexer.tok.type) {
	case TK_NULL:
		parse_error(ctx);
	case TK_IDEN:
	case TK_PUNC:
	case TK_CNST:
		handle_expr(ctx);
		break;
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
