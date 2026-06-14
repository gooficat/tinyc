#include "gen.h"
#include "error.h"
#include "lex/val.h"
#include "parse/ast.h"
#include "parse/type.h"
#include "parse/var.h"
#include "regm.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static FILE *out;
static int32_t current_frame;

static void codegen_node(ASTNode *node);
static void codegen_scope(ASTScope *scope);
static void codegen_expr(ASTNode *expr);

static int32_t find_offset(vec(CVar) vars, size_t i) {
}

static void figure_storage(ASTScope *scope, MemLoc *loc, size_t i) {
	switch (scope->vars[i].storage) {
	case STORE_IMPLICIT:
		if (scope->vars[i].type->type == TYPE_FUNC)
			loc->type = MEM_STATIC;
		else if (scope->parent) {
			loc->type = MEM_STACK;
			loc->stack = find_offset(scope->vars, i);
		} else {
			loc->type = MEM_STATIC; // TODO!!!!!! disambiguation
		}
		break;
	case STORE_AUTO:
		loc->type = MEM_STACK;
		loc->stack = find_offset(scope->vars, i);
		break;
	case STORE_TYPEDEF:
		error("This type of storage (typedef) does not belong here!!");
	case STORE_STATIC:
		loc->type = MEM_STATIC;
		break;
	case STORE_EXTERN:
		loc->type = MEM_EXTERN;
		break;
	case STORE_INLINE:
		// TODO
		break;
	}
}

static void find_memloc_rec(ASTScope *scope, MemLoc *loc, char const *name) {
	for (size_t i = 0; i < vec_len(scope->vars); ++i)
		if (!strcmp(name, scope->vars[i].name)) {
			figure_storage(scope, loc, i);
			return;
		}
	if (scope->parent)
		find_memloc_rec(scope->parent, loc, name);
	else
		loc->type = MEM_EXTERN;
}

static void find_memloc(MemLoc *loc, char const *name) {
	find_memloc_rec(current, loc, name);
}

static void codegen_cleanup_scope() {
	if (current) {
		current = current->parent;
		if (current_frame)
			fprintf(out, "\tpop %%ebp\n"); // TODO!!!
	}
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

static void codegen_args(ASTNode *arg) {
	if (arg->type == AST_CHAIN) {
		for (size_t i = vec_len(arg->chain) - 1; i >= 0; --i) {
			// TODO! big types that dont fit on eax
			codegen_expr(&arg->chain[i]);
			fprintf(out, "\tpush %%eax\n");
		}
	} else {
		codegen_expr(arg);
		fprintf(out, "\tpush %%eax\n");
	}
}

static void codegen_vref(char *name) {
	MemLoc loc;
	find_memloc(&loc, name);
	switch (loc.type) {
	case MEM_EXTERN:
	case MEM_STATIC:
		fputs(name, out);
		break;
	case MEM_STACK:
		fprintf(out, "%lld(%%ebp)", (long long)loc.stack);
		break;
	}
}

static void codegen_cnst(CConst *cnst) {
	// TODO!!!!! deal with floats not being on eax
	switch (cnst->type) {
	case CONST_INT:
		fprintf(out, "\tmov $%lld, %%eax\n", cnst->i);
		break;
	case CONST_FLT:
	case CONST_STR:
		error("Non integers not implemented");
	}
}

static void codegen_call(ASTCall *call) {
	codegen_args(call->with);
	if (call->of->type == AST_VREF) {
		fprintf(out, "\tcall ");
		codegen_vref(call->of->vref);
		fputc('\n', out);
	}
}

static void codegen_expr(ASTNode *expr) {
	switch (expr->type) {
	case AST_SCOPE:
		codegen_scope(&expr->scope);
		break;
	case AST_VREF:
		codegen_vref(expr->vref);
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
		codegen_cnst(&expr->cnst);
		break;
	case AST_CAST: // TODO allow void cast as top level expression
		error("Cast as top level expression not allowed");
		break;
	case AST_FUNC:
		break;
	}
}

static void codegen_enter_scope(ASTScope *scope) {
	if (scope->parent) {
		current = scope;
		// TODO stack frame
		current_frame = 0;
		for (size_t i = 0; i < vec_len(scope->vars); ++i)
			if (scope->vars[i].storage == STORE_AUTO)
				current_frame += WORD_WIDTH; // TODO change to a calculation

		if (current_frame)
			fprintf(out,
					"\tpush %%ebp\n"
					"\tsub $%lld, %%esp\n"
					"\tmov %%esp, %%ebp\n",
					(long long)current_frame); // TODO!!!
	}
}

static void codegen_scope(ASTScope *scope) {
	codegen_enter_scope(scope);
	for (size_t i = 0; i < vec_len(scope->children); ++i)
		codegen_node(&scope->children[i]);
	codegen_cleanup_scope();
}

static void codegen_func(ASTFunc *func) {
	fprintf(out, "%s:\n", func->name);
	codegen_scope(&func->body);
	fputs("\tret\n", out);
}

static void codegen_cast(ASTCast *cast) {
	(void)cast;
	error("Unimplemented!!");
}

static void codegen_node(ASTNode *node) {
	switch (node->type) {
	case AST_SCOPE:
		codegen_scope(&node->scope);
		break;
	case AST_VREF:
		codegen_vref(node->vref);
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
		codegen_cnst(&node->cnst);
		break;
	case AST_CAST:
		codegen_cast(&node->cast);
		break;
	case AST_FUNC:
		codegen_func(&node->func);
		break;
	}
}

void codegen_tree() {
	fprintf(out, ".section \".text\"\n");
	codegen_scope(&tree.scope);
}

void codegen_init(FILE *file) {
	out = file;
}
