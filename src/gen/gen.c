#include "gen.h"
#include "error.h"
#include "parse/ast.h"
#include <stdio.h>

static FILE *out;

static void codegen_node(ASTNode *node);
static void codegen_scope(ASTScope *scope);
static void codegen_expr(ASTNode *expr);

static void find_memloc(MemLoc *loc, char const *name) {
}

static void codegen_cleanup_scope() {
	current = current->parent;
	// free up stack frame
}

static void codegen_order(ASTOrder *order) {
	switch (order->type) {
	case ORDER_RETURN:
		codegen_expr(order->expr);
		codegen_cleanup_scope();
		fprintf(out, "\tret\n");
		break;
	case ORDER_BREAK:
		error("Unimplemented");
		break;
	case ORDER_CONTINUE:
		error("Unimplemented");
		break;
	case ORDER_GOTO:
		fprintf(out, "\tjmp %s\n", order->label); // TODO FIXME !!!! VERY IMPORTANT !!!! disambiguation
		break;
	}
}

static void codegen_call(ASTCall *call) {
	// how horrid
}

static void codegen_expr(ASTNode *expr) {
	switch (expr->type) {
	case AST_SCOPE:
		codegen_scope(&expr->scope);
		break;
	case AST_VREF:
		error("Variable reference as top level expression not allowed");
		break;
	case AST_CHAIN:
		for (size_t i = 0; i < vec_len(expr->chain); ++i)
			codegen_expr(&expr->chain[i]);
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

static void codegen_enter_scope(ASTScope *scope) {
	current = scope;
	// TODO stack frame
}

static void codegen_scope(ASTScope *scope) {
	codegen_enter_scope(scope);
	for (size_t i = 0; i < vec_len(scope->children); ++i)
		codegen_node(&scope->children[i]);
	codegen_cleanup_scope();
}

static void codegen_func(ASTFunc *func) {
	fprintf(out, "%s:\n", func->var->name);
	codegen_scope(&func->body);
}

static void codegen_node(ASTNode *node) {
	switch (node->type) {
	case AST_SCOPE:
		codegen_scope(&node->scope);
		break;
	case AST_VREF:
		break;
	case AST_CHAIN:
		for (size_t i = 0; i < vec_len(node->chain); ++i)
			codegen_expr(&node->chain[i]);
		break;
	case AST_CALL:
		codegen_call(&node->call);
		break;
	case AST_ORDER:
		codegen_order(&node->order);
		break;
	case AST_CNST:
		break;
	case AST_CAST:
		break;
	case AST_FUNC:
		codegen_func(&node->func);
		break;
	}
}

void codegen_tree() {
	current = &tree.scope;
	fprintf(out, ".section \".text\"\n");
}

void codegen_init(FILE *file) {
	out = file;
}
