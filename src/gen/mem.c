#include "mem.h"
#include "error/error.h"
#include "gen/gen.h"
#include "parse/parse.h"
#include "tiny.h"
#include "tree.h"
#include "type.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

c_sym_s *find_var_rec(char *name, ast_node_s *scope) {
	for (size_t i = 0; i < vec_len(scope->val.scope.symbols); ++i) {
		if (!strcmp(IDENTS[scope->val.scope.symbols[i].name], name)) {
			return scope->val.scope.symbols + i;
		}
	}
	if (scope->val.scope.parent) {
		return find_var_rec(name, scope->val.scope.parent);
	}
	return NULL;
}
c_sym_s *find_var(char *name) {
	return find_var_rec(name, curr_scop);
}

void find_type_of_expr(ast_node_s *node, type_s *type) {
	switch (node->type) {
	case AST_VREF: {
		*type = find_var(IDENTS[node->val.idx])->type;
		break;
	}
	case AST_CALL: {
		find_type_of_expr(node->val.call.of, type);
		*type = *type->info.fun.ret_typ;
		break;
	}
	case AST_LIST: {
		find_type_of_expr(node->val.list.elems + vec_len(node->val.list.elems) - 1, type);
		break;
	}
	case AST_UN_OP:
		find_type_of_expr(node->val.un_op.base, type);
		break;
	case AST_BIN_OP:
		find_type_of_expr(node->val.bin_op.left, type); // TODO!! check, this may be wrong
		break;
	case AST_CAST:
		*type = node->val.cast.type;
		break;
	case AST_VALUE:
		memset(type, 0, sizeof(type_s));
		switch (VALUES[node->val.idx].type) {
		case C_VAL_INT:
			type->type = TYPE_INT;
			type->info.igr.sign = SIGN_SIGNED;
			break;
		case C_VAL_FLOAT:
			type->type = TYPE_FLOAT;
			break;
		case C_VAL_STRING:
			type->type = TYPE_PTR;
			type->info.ptr = calloc(1, sizeof(type_s));
			type->info.ptr->type = TYPE_INT;
			type->info.ptr->info.igr.type = INT_CHAR;
			break;
		}
		break;
	default:
		error(ERR_CODEGEN, "Cannot do codegen on a node of this type");
	}
}
size_t calculate_sizeof_expr(ast_node_s *node) {
	type_s type;
	find_type_of_expr(node, &type);
	return calculate_sizeof(&type);
}

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
		c_sym_s *sym = node->val.scope.symbols + i;
		switch (sym->storage) {
		case STORE_IMPLICIT:
			if (is_top_scope()) {
				emit(".comm %s\n", IDENTS[sym->name]);
				break;
			}
		case STORE_AUTO:
			stack_frame += calculate_sizeof(&sym->type);
			break;
		case STORE_EXTERN:
			emit(".extern %s\n", IDENTS[sym->name]);
			break;
		case STORE_STATIC:
			if (is_top_scope()) {
				emit(".lcomm %s\n", IDENTS[sym->name]);
			} else {
				// TODO disambiguated
			}
			break;
		case STORE_TYPEDEF:
			break;
		}
	}
	if (stack_frame) {
		emit("sub %" PRIu32 ", %%esp\n", stack_frame);
	}
}

void codegen_locate_rec(char const *name, ast_node_s *scope, mem_s *loc) {
	for (size_t i = 0; i < vec_len(scope->val.scope.symbols); ++i) {
		c_sym_s *sym = scope->val.scope.symbols + i;
		// TODO disambiguation
		if (!strcmp(name, IDENTS[sym->name])) {
			return;
		}
		if (sym->storage == STORE_AUTO || (scope->val.scope.parent && sym->storage == STORE_IMPLICIT)) {
			loc->info.offs += calculate_sizeof(&sym->type);
		}
	}
	if (scope->val.scope.parent) {
		codegen_locate_rec(name, scope->val.scope.parent, loc);
	}
}

void codegen_locate(char const *name, mem_s *loc) {
	memset(loc, 0, sizeof(mem_s));
	codegen_locate_rec(name, curr_scop, loc);
}
