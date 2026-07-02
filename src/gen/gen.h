#ifndef __GEN__H__
#define __GEN__H__

#include "tree.h"
#include <stdbool.h>
#include <stdio.h>

void emit(char const *str, ...);
bool is_top_scope(void);
void codegen_init(FILE *file_);
void codegen_tree(void);
void codegen_close(void);
void codegen_expr(ast_node_s *node);

#endif //!__GEN__H__
