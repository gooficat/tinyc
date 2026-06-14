#include "parse/ast.h"
#pragma warning(disable : 4996)
#include "lex/lex.h"
#include "parse/parse.h"

void print_node(ASTNode *node) {
	switch (node->type) {
	case AST_SCOPE:
		printf("Scope, %zu vars, %zu children\n", vec_len(node->scope.vars), vec_len(node->scope.children));
		break;
	case AST_VREF:
		printf("Ref to var %s\n", node->vref->name);
		break;
	case AST_CHAIN:
		printf("Chain of %zu nodes\n", vec_len(node->chain));
		break;
	case AST_CALL:
		printf("Call\n");
		break;
	case AST_ORDER:
		printf("Order\n");
		break;
	case AST_CNST:
		printf("Constant of type %i\n", node->cnst.type);
	case AST_CAST:
		printf("Cast\n");
		break;
	case AST_FUNC:
		printf("Function of name %s\n", node->func.var->name);
		break;
	}
}

int main() {
	lexer_init(fopen("test/1.c", "r"));
	parse_tree();
	print_node(&tree);
}
