#ifndef __MEM__H__
#define __MEM__H__

#include "tree.h"

size_t calculate_sizeof(type_s *type);
size_t calculate_sizeof_struct(struct_s *struc);
void codegen_prep_frame(ast_node_s *node);
c_sym_s *codegen_locate_rec(char const *name, ast_node_s *scope);
c_sym_s *codegen_locate(char const *name);

#endif //!__MEM__H__
