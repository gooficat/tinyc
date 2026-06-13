#include "parse.h"
#include "error.h"
#include "parse/ctx.h"
#include "parse/lex/lexer.h"
#include "parse/lex/toks.h"
#include "strucs/nodes.h"
#include "strucs/symbol.h"
#include "strucs/types.h"
#include "utils/arena.h"
#include "utils/hash.h"
#include "utils/vector.h"

static void handle_stmt(struct parse_ctx *ctx);

static void init_scope(struct ast_scope *scope, struct ast_scope *parent) {
	scope->children = vec_init(struct ast_node);
	scope->vars_line = vec_init(struct c_var);
	scope->parent = parent;
	hashmap_init(&scope->vars_map);
	hashmap_init(&scope->labels_map);
	hashmap_init(&scope->types_map);
}

static void *gen_scope(struct parse_ctx *ctx) {
	struct ast_scope *scope = arena_alloc(&ctx->arena, sizeof(struct ast_scope));
	init_scope(scope, ctx->current);
	ctx->current = scope;
	lexer_next(ctx);
	while (ctx->lexer.tok.type != TK_PUNC || ctx->lexer.tok.val != PN_BRACE_R) {
		handle_stmt(ctx);
	}
	lexer_next(ctx);
	ctx->current = scope->parent;
	return scope;
}

static int decorate_node(struct parse_ctx *ctx, struct ast_node *node);

static int gen_node(struct parse_ctx *ctx, struct ast_node *node) {
	switch (ctx->lexer.tok.type) {
	case TK_PUNC:
		switch (ctx->lexer.tok.val) {
		case PN_BRACE_L: {
			node->type = AST_SCOPE;
			node->val = gen_scope(ctx);
			break;
		}
		case PN_PAREN_L:
		default:
			return 0;
		}
	case TK_CNST:
		node->type = AST_CONST;
		node->val = (void *)ctx->lexer.tok.val;
		break;
	case TK_IDEN:
	case TK_KEYW:
		/*ALWAYS a cast, this ONLY happens when we get a parentheses recursed drop-down*/
		node->type = AST_CAST;
		break;
	default:
		internal_error();
	}
	return decorate_node(ctx, node);
}

static int decorate_node(struct parse_ctx *ctx, struct ast_node *node) {
	if (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_PAREN_L) {
		struct ast_call call;
		call.target = *node;

		lexer_next(ctx); // (
		gen_node(ctx, &call.arg);
		lexer_next(ctx); // )

		node->type = AST_CALL;
		node->val = arena_alloc(&ctx->arena, sizeof(struct ast_call));
		*((struct ast_call *)node->val) = call;
	}

	if (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_COMMA) { /*chained statements.*/
		struct ast_node *chain = vec_init(struct ast_node);
		chain = vec_grow(chain, 1);
		chain[0] = *node;

		do {
			lexer_next(ctx);

			size_t i = vec_len(chain);

			chain = vec_grow(chain, 1);
			gen_node(ctx, &chain[i]);

		} while (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_COMMA);

		node->type = AST_CHAIN;
		node->val = chain;
	}

	return 1;
}

static struct hash_el *find_ident_rec(struct ast_scope *scope, char const *name) {
	struct hash_el *el = hashmap_find(&scope->vars_map, name);
	if (!el) {
		return find_ident_rec(scope->parent, name);
	}

	return el;
}

static struct c_var *find_ident(struct parse_ctx *ctx, char const *name) {
	struct hash_el *var = find_ident_rec(ctx->current, name);
	if (!var) {
		parse_error(ctx);
	}

	return var->value;
}

static void handle_expr(struct parse_ctx *ctx) {
	ctx->current->children = vec_grow(ctx->current->children, 1);
	struct ast_node *node = &ctx->current->children[vec_len(ctx->current->children) - 1];

	if (ctx->lexer.tok.type == TK_IDEN) {
		size_t name = ctx->lexer.tok.val;
		lexer_next(ctx);

		if (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_COLON) {
			lexer_next(ctx);

			struct c_label *label = arena_alloc(&ctx->arena, sizeof(struct c_label));
			label->name = ctx->identifiers[name];

			node->type = AST_LABEL;
			node->val = label;

			hashmap_insert(&ctx->current->labels_map, ctx->identifiers[name], label);
		} else {
			node->type = AST_REF;

			node->val = find_ident(ctx, ctx->identifiers[name]);
			if (!node->val) {
				parse_error(ctx);
			}

			decorate_node(ctx, node);
		}
	} else if (!gen_node(ctx, node)) {
		parse_error(ctx);
	}
	while (ctx->lexer.tok.type == TK_PUNC && ctx->lexer.tok.val == PN_SEMI) {
		lexer_next(ctx);
	}
}

static struct c_type *gen_type(struct parse_ctx *ctx) {
	struct c_type *type = malloc(sizeof(struct c_type));

	// TODO this is just assuming int with one keyword for now
	type->type = C_TYPE_INT;
	type->params = vec_init(struct c_type);
	type->width = 4;

	lexer_next(ctx);

	return type;
}

static void handle_decl(struct parse_ctx *ctx) {
	size_t idx = vec_len(ctx->current->vars_line);

	ctx->current->vars_line = vec_grow(ctx->current->vars_line, 1);
	ctx->current->vars_line[idx].type = gen_type(ctx);

	hashmap_insert(&ctx->current->vars_map, ctx->identifiers[ctx->lexer.tok.val], (void *)idx);
	lexer_next(ctx);

	if (ctx->lexer.tok.type == TK_PUNC) {
		if (ctx->lexer.tok.val == PN_PAREN_L) {
			lexer_next(ctx);
		}
	}
}

static void handle_order(struct parse_ctx *ctx) {
}

static void handle_cond(struct parse_ctx *ctx) {
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
		if (ctx->lexer.tok.val <= KW_UNION) {
			handle_decl(ctx);
			break;
		}
		if (ctx->lexer.tok.val <= KW_GOTO) {
			handle_order(ctx);
			break;
		}
		if (ctx->lexer.tok.val <= KW_IF) {
			handle_cond(ctx);
			break;
		}
		/*TODO INTRINSICS*/
		internal_error();
	}
}

void gen_tree(struct parse_ctx *ctx) {
	init_scope(&ctx->scope, NULL);
	ctx->current = &ctx->scope;
	while (ctx->lexer.tok.type != TK_NULL) {
		handle_stmt(ctx);
	}
}
