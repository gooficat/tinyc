#ifndef __TREE__H__
#define __TREE__H__

#include "lexer.h"
#include "tiny.h"
#include "tiny_c_libs/vector.h"
#include <stdint.h>

typedef struct ast_node_s ast_node_s;
typedef struct ast_scope_s ast_scope_s;

struct ast_scope_s {
	vec(ast_node_s) children;
	vec(c_sym_s) symbols;
	ast_node_s *parent;
	uint64_t disam_ctr;
};

typedef struct {
	vec(ast_node_s) elems;
} ast_list_s;

typedef struct {
	ast_node_s *of;
	ast_list_s args;
} ast_call_s;

typedef enum {
	AST_COND_IF,
	AST_COND_WHILE,
	AST_COND_FOR,
} ast_cond_e;
typedef struct {
	ast_cond_e type;
	ast_node_s *cond;  // the actual condition, like x == y
	ast_node_s *then;  // the body, like `if (cond) then`
	ast_node_s *else_; // else option. NULL if no else
} ast_cond_s;

typedef union {
	ast_node_s *expr;
	size_t idx;
} ast_order_e;
typedef struct {
	int type;
	ast_order_e val;
} ast_order_s;

typedef struct {
	size_t sym; // the symbol associated with the function
	vec(char *) labels;
	ast_node_s *body; // the function body
} ast_func_s;

typedef struct {
	tok_e op;
	ast_node_s *base;
	bool is_post; // is a postfix operator? like <var>++
} ast_un_op_s;

typedef struct {
	tok_e op;
	ast_node_s *left, *right;
} ast_bin_op_s;

typedef struct {
	type_s type;
	ast_node_s *val;
} ast_cast_s;

typedef enum {
	AST_VREF,
	AST_SCOPE,
	AST_CALL,
	AST_LIST,
	AST_COND,
	AST_ORDER,
	AST_FUNC,
	AST_UN_OP,
	AST_BIN_OP,
	AST_CAST,
	AST_VALUE,
} ast_node_e;
typedef union {
	size_t idx;
	ast_scope_s scope;
	ast_call_s call;
	ast_list_s list;
	ast_cond_s cond;
	ast_order_s order;
	ast_func_s func;
	ast_un_op_s un_op;
	ast_bin_op_s bin_op;
	ast_cast_s cast;
} ast_node_u;
struct ast_node_s {
	ast_node_e type;
	ast_node_u val;
};

#endif //!__TREE__H__
