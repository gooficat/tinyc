#include "tiny.h"
#include "gen/gen.h"
#include "lexer.h"
#include "parse/parse.h"
#include "tiny_c_libs/vector.h"
#include "tree.h"
#include <inttypes.h>
#include <stdio.h>

void print_tok(void) {
	if (tok.type == TOK_IDENT) {
		printf("IDENT `%s`\n", IDENTS[tok.val]);
	} else if (tok.type == TOK_VALUE) {
		printf("VALUE\n");
	} else {
		printf("TOK %s\n", TOKENS[tok.type]);
	}
	lexer_next();
}

void print_node(ast_node_s *node) {
	switch (node->type) {
	case AST_VREF:
		printf("Variable %s\n", IDENTS[node->val.idx]);
		break;
	case AST_SCOPE:
		printf("Scope\n");
		for (size_t i = 0; i < vec_len(node->val.scope.children); ++i) {
			print_node(&node->val.scope.children[i]);
		}
		break;
	case AST_CALL:
		printf("Call\n");
		break;
	case AST_LIST:
		printf("List\n");
		break;
	case AST_COND:
		printf("Conditional\n");
		break;
	case AST_ORDER:
		printf("Order\n");
		switch (node->val.order.type) {
		case TOK_KW_RETURN:
			print_node(node->val.order.val.expr);
			break;
		case TOK_KW_GOTO:
			printf("Label %s\n", IDENTS[node->val.order.val.idx]);
			break;
		}
		break;
	case AST_FUNC:
		printf("Function\n");
		print_node(node->val.func.body);
		break;
	case AST_UN_OP:
		printf("Unary op\n");
		break;
	case AST_BIN_OP:
		printf("Binary op\n");
		break;
	case AST_CAST:
		printf("Cast\n");
		break;
	case AST_VALUE:
		printf("Value `");
		switch (VALUES[node->val.idx].type) {
		case C_VAL_INT:
			printf("%" PRIiMAX, VALUES[node->val.idx].val.igr);
			break;
		case C_VAL_FLOAT:
			printf("%Lf", VALUES[node->val.idx].val.flt);
			break;
		case C_VAL_STRING:
			printf("%s", VALUES[node->val.idx].val.str);
			break;
		}
		fputs("`\n", stdout);
		break;
	}
}

int main(void) {
	lexer_open(fopen("./test/2.c", "r"));
	parse_tree();
	lexer_close();
	print_node(&root);
	codegen_init(fopen("./test/2.s", "w"));
	codegen_tree();
	codegen_close();
	return 0;
}
