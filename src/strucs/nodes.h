#ifndef __NODES__H__
#define __NODES__H__

#include "utils/vector.h"

struct ast_call {
	struct ast_node *target;
	struct ast_node *args;
};

struct ast_scope {
	vec(struct ast_node) children;
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
