#ifndef __GEN__H__
#define __GEN__H__

#include <stdbool.h>
#include <stdio.h>

bool is_top_scope(void);
void codegen_init(FILE *file_);
void codegen_tree(void);
void codegen_close(void);

#endif //!__GEN__H__
