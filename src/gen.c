#include "gen.h"
#include "err.h"
#include "lexer.h"
#include "val.h"
#include <stdio.h>
#include <string.h>

static FILE *out;
static struct scope *cur;

struct memloc {
	enum memtyp typ;
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
	/* this doesnt account for external and static syms
	if (scop->num_syms) {
		fprintf(out, "\tsub $%lu, %%esp\n", (unsigned long)scop->num_syms * 4); Aligned for now, TODO
	 }
	*/
	for (i = 0; i < scop->num_nodes; ++i) {
		codegen_node(&scop->nodes[i]);
	} /*
	 if (scop->num_syms) {
		 fprintf(out, "\tadd $%lu, %%esp\n", (unsigned long)scop->num_syms * 4);
	 }*/
}

static void codegen_cnst(struct cnst *cnst) {
	switch (cnst->typ) {
	case ConstNone:
		codegen_panic("No constant\n");
	case ConstInt:
		fprintf(out, "\tmov $%ld, %%eax\n", cnst->val.i);
		break;
	case ConstFloat:
		codegen_panic("Unimplemented (floats)\n");
	case ConstString:
		fprintf(out, "\tlea cnst_%zu, %%eax\n", cnst - cnsts);
		break;
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
		codegen_panic("Unimplemented (binary op)\n");
		break;
	case AstUnOp:
		codegen_panic("Unimplemented (unary op)\n");
		break;
	case AstOrder:
		switch (ast->val.order.ordr) {
		case OrderReturn:
			codegen_node(ast->val.order.val.node);
			if (cur->num_syms) {
				fprintf(out, "\tadd $%lu, %%esp\n", (unsigned long)cur->num_syms * 4);
			}
			fprintf(out, "\tret\n");
			break;
		default:
			codegen_panic("Unimplemented (order besides return)\n");
		}
		break;
	case AstFunc:
		fprintf(out,
				".globl %s\n"
				"%s:\n",
				ast->val.func.sym->name, ast->val.func.sym->name);
		codegen_scope(&ast->val.func.body);
		if (cur->num_syms) {
			fprintf(out, "\tadd $%lu, %%esp\n", (unsigned long)cur->num_syms * 4);
		}
		fprintf(out,
				"\tmov $0, %%eax\n"
				"\tret\n");
		break;
	case AstCond:
		break;
	case AstRef: {
		/*
		as a  stand-in, i'll just assume static for now
		struct memloc ml;
				find_depth(cur, ast->val.ref->name, &ml);
				if (ml.typ == MemStat)

		*/
		fprintf(out, ".extern %s\n", ast->val.ref->name);
		fprintf(out, "\tlea %s, %%eax\n", ast->val.ref->name);
	} break;
	case AstCall: {
		size_t i;
		for (i = ast->val.call.args.num_nodes; i > 0;) {
			codegen_node(&ast->val.call.args.nodes[--i]);
			fprintf(out, "\tpush %%eax\n");
		}
		codegen_node(ast->val.call.of);
		fprintf(out,
				"\tcall %%eax\n");
		if (ast->val.call.args.num_nodes) {
			fprintf(out,
					"\tadd $%lu, %%esp\n",
					(unsigned long)ast->val.call.args.num_nodes * 4);
		}
		break;

	} break;
	case AstConst:
		codegen_cnst(ast->val.cnst);
		break;
	}
}

void codegen_tree(void) {
	fprintf(out, ".section \".text\"\n");
	codegen_node(&tree);
	fprintf(out, ".section \".data\"\n");
	{
		size_t i;
		for (i = 0; i < num_cnsts; ++i) {
			if (cnsts[i].typ == ConstString) {
				fprintf(out,
						"cnst_%zu:\n"
						"\t.asciz \"%s\"\n",
						i, cnsts[i].val.s);
			}
		}
	}
}
