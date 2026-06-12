#ifndef __NODES__H__
#define __NODES__H__

#include "strucs/symbol.h"
#include "utils/hash.h"
#include "utils/pstr.h"
#include "utils/vector.h"
#include <stddef.h>


struct ast_call {
	struct ast_node *target;
	struct ast_node *args;
};

struct ast_scope {
	vec(struct ast_node) children;
	vec(struct c_var) vars_line;
	struct hash_map vars_map;
	struct ast_scope *parent;
};

struct ast_func {
	struct c_var *var;
	struct ast_scope body;
};

enum ast_order_type {
	AST_ORDER_GOTO,
};

struct ast_order {
	enum ast_order_type type;
	union {
		struct ast_node *expr;
		struct c_label *label;
	};
};

struct ast_label {
	struct pstr name;
};

enum ast_node_type {
	AST_ERR,
	AST_FUNC,
	AST_SCOPE,
	AST_ORDER,
	AST_LABEL,
};

struct ast_node {
	enum ast_node_type type;
	void *val;
};

#endif //!__NODES__H__
