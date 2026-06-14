#include "gen/gen.h"
#include "parse/ast.h"
#pragma warning(disable : 4996)
#include "lex/lex.h"
#include "parse/parse.h"

void print_node(ASTNode *node);

void print_scope(ASTScope *scope) {
	printf("Scope, %zu vars, %zu children\n", vec_len(scope->vars), vec_len(scope->children));
	for (size_t i = 0; i < vec_len(scope->vars); ++i) {
		printf("\t%s\n", scope->vars[i].name);
	}
	for (size_t i = 0; i < vec_len(scope->children); ++i) {
		print_node(&scope->children[i]);
	}
}

void print_node(ASTNode *node) {
	switch (node->type) {
	case AST_SCOPE:
		print_scope(&node->scope);
		break;
	case AST_VREF:
		printf("Ref to var %s\n", node->vref);
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
		printf("Function of name %s\n", node->func.name);
		print_scope(&node->func.body);
		break;
	}
}

int main() {
	lexer_init(fopen("test/1.c", "r"));
	parse_tree();
	print_node(&tree);
	codegen_init(fopen("test/1.s", "w"));
	codegen_tree();
}
