#include "gen.h"
#include "error/error.h"
#include "parse/parse.h"
#include "tiny.h"
#include "tree.h"
#include "type.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <vadefs.h>

static FILE *file;

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

size_t calculate_sizeof(type_s *type);

size_t calculate_sizeof_struct(struct_s *struc) {
	size_t width = 0;
	if (!struc->unified) {
		for (size_t i = 0; i < vec_len(struc->mems); ++i) {
			size_t mem_width = calculate_sizeof(&struc->mems[i]);
			mem_width += ALIGNMENT - (mem_width % ALIGNMENT);
		}
	} else {
		for (size_t i = 0; i < vec_len(struc->mems); ++i) {
			size_t mem_width = calculate_sizeof(&struc->mems[i]);
			if (mem_width > width) {
				width = mem_width;
			}
		}
	}
	return width;
}

size_t calculate_sizeof(type_s *type) {
	switch (type->type) {
	case TYPE_NONE:
	case TYPE_ERR:
		error(ERR_INTERNAL, "Trying to calculate sizeof on errorful type");
	case TYPE_INT:
		switch (type->info.igr.type) {
		case INT_PLAIN:
			return INT_WIDTH;
		case INT_CHAR:
			return 1;
		case INT_SHORT:
			return SHORT_WIDTH;
		case INT_LONG:
			return LONG_WIDTH;
		case INT_LONG_LONG:
			return LONG_LONG_WIDTH;
		}
	case TYPE_VOID:
		error(ERR_INTERNAL, "Trying to calculate sizeof on a void type");
	case TYPE_PTR:
		return PTR_WIDTH;
	case TYPE_FLOAT:
		switch (type->info.flt) {
		case FLT_PLAIN:
			return FLOAT_WIDTH;
		case FLT_DOUBLE:
			return DOUBLE_WIDTH;
		case FLT_LONG_DOUBLE:
			return LONG_DOUBLE_WIDTH;
		}
	case TYPE_STRUC:
		return calculate_sizeof_struct(&type->info.stc);
	case TYPE_ENUM:
		return INT_WIDTH;
	case TYPE_FUNC:
		return PTR_WIDTH;
	case TYPE_ARR:
		return type->info.arr.size * calculate_sizeof(type->info.arr.under);
	}
}

void codegen_prep_frame(ast_node_s *node) {
	size_t stack_frame = 0;
	for (size_t i = 0; i < vec_len(node->val.scope.symbols); ++i) {
		c_sym_s *sym = &node->val.scope.symbols[i];
		switch (sym->storage) {
		case STORE_IMPLICIT:
		case STORE_AUTO:
			stack_frame += calculate_sizeof(&sym->type);
			break;
		case STORE_EXTERN:
			emit("extern %s", sym->name);
		case STORE_STATIC:
			if (curr_scop) {
			}
		case STORE_TYPEDEF:
			break;
		}
	}
	if (stack_frame) {
		emit("sub rsp, %" PRIu32, stack_frame);
	}
}

void codegen_scope(ast_node_s *node) {
	curr_scop = node;
	codegen_prep_frame(node);
}

void codegen_tree(void) {
	codegen_scope(&root);
}

void codegen_close(void) {
}
