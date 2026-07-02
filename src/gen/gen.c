#include "gen.h"
#include "error/error.h"
#include "lexer.h"
#include "mem.h"
#include "parse/parse.h"
#include "tiny.h"
#include "tree.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <vadefs.h>

static FILE *file;

bool is_top_scope(void) {
	return !curr_scop->val.scope.parent;
}

__attribute__((format(printf, 1, 2))) void emit(char const *str, ...) {
	va_list va;
	va_start(va, str);
	vfprintf(file, str, va);
	va_end(va);
}

void codegen_init(FILE *file_) {
	file = file_;
}
void codegen_scope(ast_node_s *node);

void codegen_order(ast_node_s *node) {
	switch (node->val.order.type) {
	case TOK_KW_BREAK:
		// TODO
		error(ERR_INTERNAL, "UNIMPLEMENTED ORDER");
	case TOK_KW_RETURN:
		codegen_expr(node->val.order.val.expr);
		emit("\tret\n");
		break;
	case TOK_KW_CONTINUE:
	case TOK_KW_GOTO: // TODO disambiguated symbols
		error(ERR_INTERNAL, "UNIMPLEMENTED ORDER");
	}
}

void codegen_var(char const *const name) {
	mem_s loc;
	codegen_locate(name, &loc);
	switch (loc.type) {
	case MEM_UNFOUND:
		// TODO
		error(ERR_INTERNAL, "Extern memory implicitness unimplemented");
	case MEM_EXTERN:
	case MEM_STATIC:
		emit("%s", name);
	case MEM_STACK:
		emit("%" PRIi64 "(%%esp)", loc.info.offs);
		break;
	case MEM_REGISTER:
		// TODO
		break;
	}
}

void codegen_call(ast_node_s *node) {
	emit("\tpush %%ebp\n");
	for (size_t i = vec_len(node->val.call.args.elems) - 1; i >= 0; --i) {
		ast_node_s *param = node->val.call.args.elems + i;
		size_t size = calculate_sizeof_expr(param);
		if (param->type == AST_VREF) {
			emit("\tpush ");
			codegen_var(IDENTS[param->val.idx]);
			emit("\n");
		} else {
			codegen_expr(param);
			if (size <= ALIGNMENT) {
				emit("\tpush %%eax\n");
			}
		}
		// TODO this solution is horrid. use a different approach
	}
	emit("mov %%esp, %%ebp\n");
	if (node->val.call.of == AST_VREF) {
		emit("\tcall ");
		codegen_var(IDENTS[node->val.call.of->val.idx]);
		emit("\n");
	} else {
		codegen_expr(node->val.call.of);
		emit("\tcall %%eax\n");
		// TODO error check for type of expr being called...
	}
	emit("\tpop %%ebp\n"
		 "\tmov %%ebp, %%esp\n");
}

void codegen_func(ast_node_s *node) {
	emit("%s:\n", IDENTS[node->val.func.sym]);
	codegen_expr(node->val.func.body);
	// emit("ret\n"); TODO add this only for functions where not all paths return a value
}

void codegen_expr(ast_node_s *node) {
	switch (node->type) {
	case AST_VREF:
	case AST_SCOPE:
		codegen_scope(node);
		break;
	case AST_CALL:
		codegen_call(node);
		break;
	case AST_LIST:
	case AST_COND:
		break;
	case AST_ORDER:
		codegen_order(node);
		break;
	case AST_FUNC:
		codegen_func(node);
		break;
	case AST_UN_OP:
	case AST_BIN_OP:
	case AST_CAST:
		break;
	case AST_VALUE:
		switch (VALUES[node->val.idx].type) {
		case C_VAL_INT:
			emit("\tmov $%" PRIiMAX ", %%eax\n", VALUES[node->val.idx].val.igr);
			break;
		case C_VAL_FLOAT:
			emit("\tmov $%Lf %%eax\n", VALUES[node->val.idx].val.flt);
			break;
		case C_VAL_STRING: // TODO assign a unique value to the strings
			error(ERR_INTERNAL, "Unimplemented");
		}
		break;
	}
}

void codegen_scope(ast_node_s *node) {
	curr_scop = node;
	codegen_prep_frame(node);
	for (size_t i = 0; i < vec_len(node->val.scope.children); ++i) {
		codegen_expr(node->val.scope.children + i);
	}
	curr_scop = node->val.scope.parent;
}

void codegen_tree(void) {
	emit(".section \".text\"\n");
	codegen_scope(&root);
}

void codegen_close(void) {
	fclose(file);
}
