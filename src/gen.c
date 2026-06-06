#include "gen.h"
#include "err.h"
#include <corecrt_wstdio.h>
#include <stdio.h>
#include <string.h>

static FILE *out;
static struct scope *cur;

struct memloc {
	enum {
		MemNone,
		MemErr,
		MemStk,
		MemStat,
		MemExtrn
	} typ;
	union {
		unsigned long dep;
	} val;
};

void codegen_init(FILE *file) {
	out = file;
}

/*eventually labels may need a similar func*/
static unsigned long rec_find_depth(struct scope *scop, char const *sym, struct memloc *ml, unsigned long dep) {
	size_t i;
	for (i = 0; i < scop->num_syms; ++i) {
		if (!strcmp(sym, scop->syms[i].name)) {
			return dep;
		}
		dep += 4; /*32 bit (4 byte) width*/
	}
	return rec_find_depth(scop->parnt, sym, ml, dep);
}

static void find_depth(struct scope *scop, char const *sym, struct memloc *ml) {
	ml->typ = MemNone;
	unsigned long i = 0;
	while (!ml->typ) {
		i = rec_find_depth(scop, sym, ml, i);
	}
}

static void codegen_scope(struct scope *scop) {
	size_t i;
	cur = scop;
	if (scop->num_syms) {
		fprintf(out, "sub $%lu, %%esp\n", (unsigned long)scop->num_syms * 4); /*Aligned for now, TODO*/
	}
	for (i = 0; i < scop->num_nodes; ++i) {
		codegen_node(&scop->nodes[i]);
	}
	if (scop->num_syms) {
		fprintf(out, "add $%lu, %%esp\n", (unsigned long)scop->num_syms * 4);
	}
}

static void codegen_cnst(struct cnst *cnst) {
	switch (cnst->typ) {
	case ConstNone:
		codegen_panic("No constant\n");
	case ConstInt:
		fprintf(out, "mov $%ld, %%eax\n", cnst->val.i);
		break;
	case ConstFloat:
	case ConstString:
		codegen_panic("Unimplemented\n");
	}
}

/*simple, unoptimized model for now*/
void codegen_node(struct ast *ast) {
	switch (ast->typ) {
	case AstNone:
		break;
	case AstScope:
		codegen_scope(&ast->val.scope);
		break;
	case AstBinOp:
		codegen_panic("Unimplemented\n");
		break;
	case AstUnOp:
		codegen_panic("Unimplemented\n");
		break;
	case AstOrder:
		switch (ast->val.order.ordr) {
		case OrderReturn:
			codegen_node(ast->val.order.val.node);
			if (cur->num_syms) {
				fprintf(out, "add $%lu, %%esp\n", (unsigned long)cur->num_syms * 4);
			}
			fprintf(out, "ret\n");
			break;
		default:
			codegen_panic("Unimplemented\n");
		}
		break;
	case AstFunc:
		fprintf(out,
				".globl %s\n"
				"%s:\n",
				ast->val.func.sym->name, ast->val.func.sym->name);
		codegen_scope(&ast->val.func.body);
		if (cur->num_syms) {
			fprintf(out, "add $%lu, %%esp\n", (unsigned long)cur->num_syms * 4);
		}
		fprintf(out,
				"mov $0, %%eax\n"
				"ret\n");
		break;
	case AstCond:
		break;
	case AstRef:
		break;
	case AstConst:
		codegen_cnst(ast->val.cnst);
		break;
	}
}

void codegen_tree(void) {
	fprintf(out, ".section \".text\"\n");
	codegen_node(&tree);
}
