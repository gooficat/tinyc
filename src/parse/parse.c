#include "parse.h"
#include "ast.h"
#include "lex/lex.h"
#include "type.h"
#include "utils/vector.h"
#include "var.h"

ASTNode tree;
static ASTScope *current;

static Type *parse_type() {
	return NULL;
}

static void init_scope(ASTScope *scope) {
	scope->children = vec_init(ASTNode);
	scope->parent = current;
	scope->vars = vec_init(Var);

	current = scope;
}

static void handle_decl() {
}

static void handle_order() {
}

static void handle_keyword() {
	if (kw_is_type()) {
		handle_decl();
	} else if (kw_is_order()) {
		handle_order();
	} else {
		// TODO unimplemented...
	}
}

static void handle_expr() {
}

static void handle_stmt() {
	if (token.type == TOK_KEYW) {
		handle_keyword();
	} else if (token.type == TOK_CNST) {
		// Terry Davis smh
	} else if (tok_is(TOK_PUNC, PN_SEMI)) {
		lexer_next();
	} else {
		handle_expr();
	}
}

void parse_tree() {
	tree.type = AST_SCOPE;
	init_scope(&tree.scope);

	while (token.type != TOK_NONE) {
		handle_stmt();
	}
}
