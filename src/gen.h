#ifndef __GEN__H__
#define __GEN__H__

#include "tree.h"
#include <stdio.h>

/*enum reg {

};*/

void codegen_init(FILE *file);
void codegen_node(struct ast *ast);
void codegen_tree(void);

#endif
