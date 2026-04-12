#ifndef __NODS__H__
#define __NODS__H__

#include "tokns.h"
#include "types.h"
#include "vectr.h"
typedef struct ast_node ast_node;
vec_type(ast_node);

typedef struct
{
	unsigned long long argc;
} ast_func_sym_info;

typedef enum
{
	AST_SYM_STATIC,
	AST_SYM_NORMAL,
	AST_SYM_EXTERN,
	AST_SYM_LABEL,
} ast_sym_cat;
typedef struct
{
	ast_sym_cat cat;
	char	   *name;
	type	   *type;
	vec_type	args;
} ast_sym;
vec_type(ast_sym);

typedef enum
{
	AST_ERR,
	AST_UN_OP,
	AST_BIN_OP,
	AST_SYM_REF,
	AST_LITERAL,
	AST_ORDER,
	AST_COND,
} ast_node_type;

typedef enum
{
	NUMERIC_LITERAL,
	FLOATING_LITERAL,
	ARRAY_LITERAL,
	STRING_LITERAL,
} ast_literal_cat;

typedef struct
{
	long long val;
} ast_literal_int;

typedef struct
{
	char *val;
} ast_literal_str;

typedef struct
{
	ast_literal_cat cat;
	type		   *type;
	union
	{
		ast_literal_int i;
		ast_literal_str s;
	};
} ast_literal;
typedef struct
{
	ast_node *node;
	operator op;
} ast_un_op;
typedef struct
{
	ast_node *left;
	ast_node *right;
	operator op;
} ast_bin_op;
typedef struct
{
	ast_sym *sym;
} ast_sym_ref;
typedef struct ast_scope ast_scope;
struct ast_scope
{
	vec_ast_sym	 syms;
	vec_ast_node nodes;
	vec_type	 types;
	ast_scope	*parent;
};

typedef struct
{
	ast_sym	 *sym;
	ast_scope body;
} ast_func;

typedef enum
{
	AST_ORDER_RETURN,
	AST_ORDER_CONTINUE,
	AST_ORDER_BREAK,
	AST_ORDER_GOTO,
} ast_order_cat;
typedef struct
{
	ast_order_cat cat;
} ast_order;

typedef enum
{
	AST_COND_IF,
	AST_COND_WHILE,
	AST_COND_FOR,
	AST_COND_DO_WHILE,
} ast_cond_cat;
typedef struct
{
	ast_cond_cat cat;
	ast_node	*cond;
	ast_node	*body;
} ast_cond;

struct ast_node
{
	ast_node_type type;
	void		 *node;
};

#endif //!__NODS__H__
