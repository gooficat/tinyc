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

void emit(char const *str, ...) {
	va_list va;
	va_start(va, str);
	fprintf(file, str, va);
	fflush(file);
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

void codegen_call(ast_node_s *node) {
	for (size_t i = vec_len(node->val.call.args.elems) - 1; i >= 0; --i) {
		ast_node_s *param = node->val.call.args.elems + i;
		size_t size = calculate_sizeof_expr(param);
		if (param->type == AST_VREF) {
			mem_s loc;
			codegen_locate(IDENTS[param->val.idx], &loc);
			switch (loc.type) {

			case MEM_UNFOUND:
			case MEM_EXTERN:
			case MEM_STACK:
			case MEM_STATIC:
			case MEM_REGISTER:
				break;
			}
		} else {
			if (size <= ALIGNMENT) {
				emit("push eax");
			}
		}
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
	codegen_scope(&root);
}

void codegen_close(void) {
	fclose(file);
}
