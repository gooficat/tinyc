#include "parse.h"
#include "ast.h"
#include "lex/lex.h"
#include "lex/tok.h"
#include "type.h"
#include "utils/vector.h"
#include "var.h"
#include <stdlib.h>

ASTNode tree;
static ASTScope *current;

static void modify_type(Type *type) {
	switch (token.indx) {
	case KW_INT:
		type->type = TYPE_INT;
		break;
	case KW_CHAR:
		type->type = TYPE_INT;
		type->igr.precision = INT_CHAR;
		break;
	case KW_SHORT:
		type->igr.precision = INT_SHORT;
		break;
	case KW_LONG:
		if (type->igr.precision == INT_LONG)
			type->igr.precision = INT_LONG_LONG;
		else
			type->igr.precision = INT_LONG;
		break;
	case KW_FLOAT:
		type->flt.precision = FLT_FLOAT;
		break;
	case KW_DOUBLE:
		// TODO not quite right
		type->flt.precision = FLT_DOUBLE;
		break;
	}
}

static Type *parse_type() {
	Type *type = malloc(sizeof(Type));

	while (token.type == TOK_KEYW && kw_is_type()) {
		modify_type(type);
	}

	return type;
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
	if (kw_is_type() || kw_is_storage()) {
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
