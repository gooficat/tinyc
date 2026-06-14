#include "parse.h"
#include "ast.h"
#include "error.h"
#include "lex/lex.h"
#include "lex/tok.h"
#include "type.h"
#include "utils/vector.h"
#include "var.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

ASTNode tree;
static ASTScope *current;

static void gen_node(ASTNode *node);
static void handle_stmt();

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
	case KW_DOUBLE: // TODO not quite right
		if (type->igr.precision == INT_LONG)
			type->flt.precision = FLT_DOUBLE;
		else
			type->flt.precision = FLT_DOUBLE;
		break;
	}
}

static Type *parse_type() {
	Type *type = calloc(1, sizeof(Type));

	while (token.type == TOK_KEYW && kw_is_type()) {
		modify_type(type);
	}

	return type;
}

static void init_scope(ASTScope *scope) {
	scope->children = vec_init(ASTNode);
	scope->vars = vec_init(Var);
	scope->types = vec_init(TypeDef);
	scope->tagged = vec_init(TypeDef);
	scope->labels = vec_init(Label);
	scope->parent = current;
	current = scope;
}

static void add_typedef(Type *type, char *name) {
	size_t idx = vec_len(current->types);
	current->types = vec_grow(current->types, 1);
	current->types[idx].type = type;
	current->types[idx].name = name;
}

static void add_var(Type *type, char const *name, Storage storage) {
	size_t idx = vec_len(current->vars);
	current->vars = vec_grow(current->vars, 1);
	current->vars[idx].name = name;
	current->vars[idx].type = type;
	current->vars[idx].storage = storage;
}

static void add_child(ASTNode *node) {
	size_t idx = vec_len(current->children);
	current->children = vec_grow(current->children, 1);
	current->children[idx] = *node;
}

static int parse_storage() {
	return token.indx; // the 2 enums are equivalent
}

static void handle_decl() {
	Type *type = calloc(1, sizeof(Type));
	Storage storage = -1;

	while (token.type == TOK_KEYW) {
		if (kw_is_storage()) {
			storage = parse_storage();
			lexer_next();
		} else if (kw_is_type()) {
			modify_type(type);
		} else {
		}
	}

	if (storage == -1) {
		if (current->parent) {
			storage = STORE_AUTO;
		} else {
			storage = STORE_STATIC;
		}
	}

	if (storage == STORE_TYPEDEF) {
		if (token.type != TOK_IDEN) {
			error("Typedef expects identifier");
		}
		add_typedef(type, token.iden);
		lexer_next();
	} else {
		add_var(type, token.iden, storage);
		lexer_next();
	}
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

Var *find_var_rec(char const *name, ASTScope *current) {
	for (size_t i = 0; i < vec_len(current->vars); ++i) {
		if (!strcmp(name, current->vars[i].name)) {
			return &current->vars[i];
		}
	}
	if (current->parent) {
		return find_var_rec(name, current->parent);
	}
	add_var(NULL, name, STORE_EXTERN);
}

Var *find_var(char const *name) {
	return find_var_rec(name, current);
}

static void gen_scope(ASTNode *node) {
	init_scope(&node->scope);
	lexer_next();
	while (!tok_is(TOK_PUNC, PN_BRACE_R)) {
		handle_stmt();
	}
	lexer_next();
	current = current->parent;
}

static void gen_expr(ASTNode *node) {
	gen_node(node);
	// TODO chains
	if (tok_is(TOK_PUNC, PN_COMMA)) {
		vec(ASTNode) chain = vec_init(ASTNode);
		chain[0] = *node;

		do {
			lexer_next();
			size_t i = vec_len(chain);
			chain = vec_grow(chain, 1);
			gen_expr(&chain[i]);
		} while (tok_is(TOK_PUNC, PN_COMMA));

		node->type = AST_CHAIN;
		node->chain = chain;
	}
}

static void gen_cast(ASTNode *node) {
	node->type = AST_CAST;
	node->cast.to = parse_type();
}

static void gen_punc(ASTNode *node) {
	switch (token.indx) {
	case PN_PAREN_L:
		lexer_next();
		if (token.type == TOK_KEYW && kw_is_type()) {
			gen_cast(node);
		} else {
			gen_expr(node);
		}
		if (tok_is(TOK_PUNC, PN_PAREN_R)) {
			lexer_next();
			break;
		} else {
			error("Unclosed parentheses");
		}
	case PN_BRACE_L:
		gen_scope(node);
		break;
	default:
		error("Unexpected punctuation");
	}
}

static void gen_node(ASTNode *node) {
	switch (token.type) {
	case TOK_NONE:
		error("Unexpected end of input");
	case TOK_PUNC:
		gen_punc(node);
		break;
	case TOK_KEYW:
		error("Unimplemented");
	case TOK_IDEN:
		node->type = AST_VREF;
		node->vref = find_var(token.iden);
		break;
	case TOK_CNST:
		node->type = AST_CNST;
		node->cnst = token.cnst;
		break;
	}
	if (tok_is(TOK_PUNC, PN_PAREN_L)) {
		lexer_next();
	}
}

static void handle_expr() {
	ASTNode node;
	gen_expr(&node);
	add_child(&node);
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
