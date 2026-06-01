#include "ast.h"
#include "lex.h"
#include <stddef.h>
#include <stdio.h>

void PrintSymbol(CSymbol *sym) {
	printf("Symbol named %s\n", sym->name);
}

void PrintTree(ASTNode *node) {
	switch (node->type) {
	case AST_NONE:
		printf("Failure -- empty node\n");
		return;
	case AST_SCOPE: {
		size_t i;
		printf("Scope");
		for (i = 0; i < dalen(node->val.scope.members); ++i) {
		}
		return;
	}
	case AST_BINOP:
		printf("Binary op\nLeft:\n");
		PrintTree(node->val.binop.l);
		printf("Right:\n");
		PrintTree(node->val.binop.r);
		return;
	case AST_UNOP:
		printf("Unary op\nNode:\n");
		PrintTree(node->val.unop.n);
		return;
	case AST_CALL:
		printf("Call of:\n");
		PrintTree(node->val.call.of);
		return;
	case AST_FUNC: {
		size_t i;
		printf("Function\n");
		PrintSymbol(node->val.func.symbol);
		printf("Body:\n");
		for (i = 0; i < dalen(node->val.scope.members); ++i) {
			PrintTree(&node->val.func.body.members[i]);
		}
		return;
	}
	case AST_ORDER:
		printf("Order\n");
		return;
	case AST_CONST:
		printf("Constant\n");
		return;
	case AST_REF:
		printf("Reference to var %s\n", node->val.ref->name);
		return;
	}
}

int main(void) {
	Lexer	lexer;
	ASTNode root;
	Lexer_Init(&lexer, fopen("test/1.c", "rt"));
	ParseTree(&root, &lexer);
	PrintTree(&root);

	return 0;
}
