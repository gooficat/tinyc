#ifndef __PARSE__H__
#define __PARSE__H__

#include "tree.h"

void init_scope(ast_node_s *scope);
void add_node(ast_node_s *node);
void handle_stmt(void);
void mod_type(type_s *type);

extern ast_node_s root;
extern ast_node_s *curr_scop;

#endif //!__PARSE__H__
