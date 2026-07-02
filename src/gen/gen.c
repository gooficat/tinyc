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
	fprintf(file, str, va);
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
		// TODO
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
	for (size_t i = vec_len(node->val.call.args.elems) - 1; i >= 0; --i) {
		ast_node_s *param = node->val.call.args.elems + i;
		size_t size = calculate_sizeof_expr(param);
		if (param->type == AST_VREF) {
			emit("push ");
			codegen_var(IDENTS[param->val.idx]);
			emit("\n");
		} else {
			codegen_expr(param);
			if (size <= ALIGNMENT) {
				emit("push %%eax\n");
			}
		}
	}
	if (node->val.call.of == AST_VREF) {
		emit("call ");
		codegen_var(IDENTS[node->val.call.of->val.idx]);
		emit("\n");
	} else {
		codegen_expr(node->val.call.of);
		emit("call %%eax\n");
		// TODO error check for type of expr being called...
	}
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
	case AST_ORDER:
		codegen_order(node);
		break;
	case AST_FUNC:
	case AST_UN_OP:
	case AST_BIN_OP:
	case AST_CAST:
	case AST_VALUE:
		break;
	}
}

void codegen_scope(ast_node_s *node) {
	curr_scop = node;
	codegen_prep_frame(node);
	for (size_t i = 0; i < vec_len(node->val.scope.children); ++i) {
		codegen_expr(node->val.scope.children + i);
	}
}

void codegen_tree(void) {
	emit(".section \".text\"\n");
	codegen_scope(&root);
}

void codegen_close(void) {
	fclose(file);
}
