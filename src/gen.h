#ifndef __GEN__H__
#define __GEN__H__

#include "tree.h"
#include <stdio.h>

/*enum reg {

};*/

void codegen_init(FILE *file);
void codegen_node(struct ast *ast);
void codegen_tree(void);

#define CHAR_SIZE 1
#define SHORT_SIZE 2
#define INT_SIZE 4
#define LONG_SIZE 8

#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8
#define LONG_DOUBLE_SIZE 8

#define PTR_SIZE 4

#define ALIGN_SIZE 4

#endif
