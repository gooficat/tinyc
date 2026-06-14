#include "gen.h"
#include "error.h"
#include "parse/ast.h"
#include <stdio.h>

static FILE *out;

void codegen_node(ASTNode *node);
void codegen_scope(ASTScope *scope);

void codegen_order(ASTOrder *order) {
}

void codegen_call(ASTCall *call) {
}

void codegen_expr(ASTNode *expr) {
	switch (expr->type) {
	case AST_SCOPE:
		codegen_scope(&expr->scope);
		break;
	case AST_VREF:
		error("Variable reference as top level expression not allowed");
		break;
	case AST_CHAIN:
		for (size_t i = 0; i < vec_len(expr->chain); ++i) {
			codegen_expr(&expr->chain[i]);
		}
		break;
	case AST_CALL:
		codegen_call(&expr->call);
		break;
	case AST_ORDER:
		codegen_order(&expr->order);
		break;
	case AST_CNST:
		error("Constant as top level expression not allowed");
	case AST_CAST: // TODO allow void cast as top level expression
		error("Cast as top level expression not allowed");
		break;
	case AST_FUNC:
		break;
	}
}

void codegen_scope(ASTScope *scope) {
	// TODO stack frame
	for (size_t i = 0; i < vec_len(scope->children); ++i) {
		codegen_node(&scope->children[i]);
	}
	// free up stack frame
}

void codegen_node(ASTNode *node) {
}

void codegen_tree() {
	fprintf(out, "");
}

void codegen_init(FILE *file) {
	out = file;
}
