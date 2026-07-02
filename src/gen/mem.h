#ifndef __MEM__H__
#define __MEM__H__

#include "tree.h"

typedef enum {
	MEM_UNFOUND,
	MEM_EXTERN,
	MEM_STACK,
	MEM_STATIC,
	MEM_REGISTER,
} mem_e;
typedef union {
	int64_t offs;
	char *name;
	// TODO register
} mem_u;
typedef struct {
	mem_e type;
	mem_u info;
} mem_s;

size_t calculate_sizeof(type_s *type);
size_t calculate_sizeof_struct(struct_s *struc);
void codegen_prep_frame(ast_node_s *node);
void codegen_locate_rec(char const *name, ast_node_s *scope, mem_s *loc);
void codegen_locate(char const *name, mem_s *loc);
void find_type_of_expr(ast_node_s *node, type_s *type);
size_t calculate_sizeof_expr(ast_node_s *node);

#endif //!__MEM__H__
