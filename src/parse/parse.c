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
ASTScope *current;

static void gen_node(ASTNode *node);
static void gen_expr(ASTNode *node);
static void handle_stmt();
static void gen_scope(ASTNode *node);

static void modify_type(CType *type) {
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
	lexer_next();
}

static CType *parse_type() {
	CType *type = calloc(1, sizeof(CType));
	while (token.type == TOK_KEYW && kw_is_type()) {
		modify_type(type);
	}

	return type;
}

static void init_scope(ASTScope *scope) {
	scope->children = vec_init(ASTNode);
	scope->vars = vec_init(CVar);
	scope->types = vec_init(CTypeDef);
	scope->tagged = vec_init(CTypeDef);
	scope->labels = vec_init(Label);
	scope->parent = current;
	current = scope;
}

static void add_typedef(CType *type, char *name) {
	size_t idx = vec_len(current->types);
	current->types = vec_grow(current->types, 1);
	current->types[idx].type = type;
	current->types[idx].name = name;
}

static void add_var(CType *type, char const *name, Storage storage) {
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

static void build_var_decl(CVar *var) {
	// TODO!!!! Deal with storage specifiers
	var->type = parse_type();

	if (token.type == TOK_IDEN) {
		var->name = token.iden;
		lexer_next();
	}
}

static void handle_func(CVar *var) {
	ASTNode func;
	func.type = AST_FUNC;
	init_scope(&func.func.body);
	for (size_t i = 0; i < vec_len(var->type->func.params); ++i) {
		vec_grow(func.func.body.vars, 1);
		func.func.body.vars[i] = var->type->func.params[i];
	}
	lexer_next();
	while (!tok_is(TOK_PUNC, PN_BRACE_R)) {
		handle_stmt();
	}
	lexer_next();
	current = current->parent;

	add_child(&func);
}

static void handle_decl() {
	CType *type = calloc(1, sizeof(CType));
	char *name;
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
	name = token.iden;
	lexer_next();

	if (tok_is(TOK_PUNC, PN_PAREN_L)) {
		CType *new = calloc(1, sizeof(CType));

		new->type = TYPE_FUNC;
		new->func.ret_type = type;
		new->func.params = vec_init(CVar);
		type = new;

		lexer_next();
		while (!tok_is(TOK_PUNC, PN_PAREN_R)) {
			size_t i = vec_len(new->func.params);
			new->func.params = vec_grow(new->func.params, 1);
			build_var_decl(&new->func.params[i]);
			lexer_next();
		}
		lexer_next();
	}

	if (storage == STORE_TYPEDEF) {
		if (token.type != TOK_IDEN) {
			error("CTypedef expects identifier");
		}
		add_typedef(type, name);
	} else {
		add_var(type, name, storage);

		if (tok_is(TOK_PUNC, PN_BRACE_L)) {
			if (type->type != TYPE_FUNC) {
				error("Not a function declaration");
			}
			handle_func(&current->vars[vec_len(current->vars) - 1]);
		}
	}
}

Label *find_label_rec(char const *name, ASTScope *current) {
	for (size_t i = 0; i < vec_len(current->labels); ++i) {
		if (!strcmp(name, current->labels[i].name)) {
			return &current->labels[i];
		}
	}
	if (current->parent) {
		return find_label_rec(name, current->parent);
	}
	error("Unknown label");
}

Label *find_label(char const *name) {
	return find_label_rec(name, current);
}

OrderCType parse_order_type() {
	return token.indx;
}

static void handle_order() {
	ASTNode node;
	node.order.type = parse_order_type();
	lexer_next();

	switch (node.order.type) {
	case ORDER_RETURN:
		node.order.expr = malloc(sizeof(ASTNode));
		gen_expr(node.order.expr);
		break;
	case ORDER_BREAK: // Potential future upgrade: add a multibreak to replace nested-loop goto shenanigans
		break;
	case ORDER_CONTINUE:
		break;
	case ORDER_GOTO:
		node.order.label = find_label(token.iden);
		lexer_next();
		break;
	}
	add_child(&node);
}

static void handle_keyword() {
	if (kw_is_type() || kw_is_storage()) {
		handle_decl();
	} else if (kw_is_order()) {
		handle_order();
	} else {
		// TODO
		error("unimplemented");
	}
}

CVar *find_var_rec(char const *name, ASTScope *current) {
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

CVar *find_var(char const *name) {
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
		lexer_next();
		break;
	case TOK_CNST:
		node->type = AST_CNST;
		node->cnst = token.cnst;
		lexer_next();
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
		// maybe later
		error("Token at top level of statement is not allowed");
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
