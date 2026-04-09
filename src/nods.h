#ifndef __NODS__H__
#define __NODS__H__

#include "tokns.h"
#include "vectr.h"
typedef struct ast_node ast_node;
vec_type(ast_node);

typedef struct
{
	char *name;
} ast_sym;

typedef enum
{
	AST_ERR,
	AST_UN_OP,
	AST_BIN_OP,
	AST_SYM_REF,
} ast_node_type;

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

} ast_sym_ref;
typedef struct
{
	vec_ast_node nodes;
} ast_scope;

struct ast_node
{
	ast_node_type *type;
	void		  *node;
};

typedef struct
{

} ast_ctx;

#endif //!__NODS__H__
