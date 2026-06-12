#ifndef __PARSE__H__
#define __PARSE__H__

#include "parse/lex/lexer.h"
#include "strucs/nodes.h"

void gen_tree(struct ast_node *node, struct lexer *lexer);

#endif //!__PARSE__H__
