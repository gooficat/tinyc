#include "gen.h"
#include "dict.h"
#include "err.h"
#include "lexer.h"
#include "val.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vadefs.h>
#include <vcruntime.h>

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
			ml->typ = scop->syms[i].mem;
			return dep;
		}
		dep += ALIGN_SIZE;
	}
	return rec_find_depth(scop->parnt, sym, ml, dep);
}

static void find_depth(struct scope *scop, char const *sym, struct memloc *ml) {
	unsigned long i = 0;
	ml->typ = MemNone;
	while (!ml->typ) {
		i = rec_find_depth(scop, sym, ml, i);
	}
	ml->val.dep = i;
}

void gen_lvalue(struct ast *ast) {
	if (ast->typ == AstUnOp) {
		/**/
		return;
	}
	if (ast->typ == AstRef) {
		struct memloc ml;
		find_depth(cur, ast->val.ref->name, &ml);
		switch (ml.typ) {
		case MemStk:
			fprintf(out, "\tlea %ld(%%esp), %%eax\n", ml.val.dep);
			break;
		case MemExtrn:
			fprintf(out, ".extern %s\n", ast->val.ref->name);
		case MemStat:
			fprintf(out, "\tlea %s, %%eax\n", ast->val.ref->name);
			break;
		default:
			codegen_panic("Invalid mem type\n");
		}
		return;
	}
}

static void codegen_binop(struct ast *ast) {
	/*
	TODO assignments, structs. Parser will need a rework to allow structs.
	May it be best to treat it as punctuation instead of operators?
*/
	if (ast->val.binop.operator == OpAss) {
		/*They are just stack and static now so no need for this. besides, if i do registers, an enum + table may make more sense		char rvbuf[RVAL_MAX];*/
		gen_lvalue(ast->val.binop.left);
		fprintf(out, "\tpush %%eax\n"); /*TODO non-register values*/
		codegen_node(ast->val.binop.right);
		fprintf(out, "\tpop %%edx\n"); /*TODO non-register values*/
		fprintf(out, "\tmov %%eax, (%%edx)\n");
		return;
	} /*
		  else if (ast->val.binop.operator == OpDot) {

	 } else if (ast->val.binop.operator == OpArrow) {
	 }*/
	codegen_node(ast->val.binop.left);
	fprintf(out, "\tpush %%eax\n");
	codegen_node(ast->val.binop.right);
	fprintf(out, "\tpop %%edx\n");
	switch (ast->val.binop.operator) {
	case OpShR:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpShL:
		fprintf(out, "\tshl %%edx, %%eax\n");
		break;
	case OpAdd:
		fprintf(out, "\tadd %%edx, %%eax\n");
		break;
	case OpSub:
		fprintf(out, "\tsub %%edx, %%eax\n");
		break;
	case OpMul:
		fprintf(out, "\timul %%edx, %%eax\n");
		break;
	case OpDiv:
		fprintf(out, "\tidiv\n");
		break;
	case OpMod:
		fprintf(out, "\tidiv %%edx, %%eax, %%eax\n");
		break;
		/*
	case OpLogAnd:
		fprintf(out, "\tand %%edx, %%eax\n");
		break;
	case OpLogOr:
		fprintf(out, "\tor %%edx, %%eax\n");
		break;
	case OpLogEqu:
		fprintf(out, "\tsub %%edx, %%eax\n");
		break;
	case OpLogGEqu:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpLogLEqu:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpBinAndEqu:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpBinOrEqu:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpBinXOrEqu:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpLogGrtr:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;
	case OpLogLess:
		fprintf(out, "\tshr %%edx, %%eax\n");
		break;*/
	case OpBinAnd:
		fprintf(out, "\tand %%edx, %%eax\n");
		break;
	case OpBinOr:
		fprintf(out, "\tor %%edx, %%eax\n");
		break;
	case OpBinXOr:
		fprintf(out, "\txor %%edx, %%eax\n");
		break;
	case OpBinNot:
		fprintf(out, "\tnot %%edx, %%eax\n");
		break;
	default:
		codegen_panic("Error! Not a binary operator\n");
	}
}

static unsigned long scope_stack(struct scope *scop) {
	unsigned long s;
	size_t i;
	s = 0;
	for (i = 0; i < scop->num_syms; ++i) {
		if (scop->syms[i].mem == MemStk) {
			s += ALIGN_SIZE; /*TODO bigger symbols*/
		}
	}
	return s;
}

static void codegen_scope(struct scope *scop) {
	size_t i;
	cur = scop;
	/* this doesnt account for external and static syms*/
	i = scope_stack(scop);
	if (i) {
		fprintf(out, "\tsub $%lu, %%esp\n", (unsigned long)i); /*Aligned for now, TODO*/
	}
	for (i = 0; i < scop->num_nodes; ++i) {
		codegen_node(&scop->nodes[i]);
	}
	i = scope_stack(scop);
	if (i) {
		fprintf(out, "\tadd $%lu, %%esp\n", (unsigned long)i);
	}
	cur = scop->parnt;
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
		fprintf(out, "\tlea cnst_%zu, %%eax\n", cnsts - cnst);
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
	case AstBinOp: {
		codegen_binop(ast);
	} break;
	case AstUnOp:
		codegen_panic("Unimplemented (unary op)\n");
		break;
	case AstOrder:
		switch (ast->val.order.ordr) {
		case OrderReturn:
			codegen_node(ast->val.order.val.node);
			{
				unsigned long i = scope_stack(cur);
				if (i) { /*TODO account for statics*/
					fprintf(out, "\tadd $%lu, %%esp\n",
							i);
					/*TODO bigger types*/
					/*TODO make sure bigger types count*/
				}
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
		fprintf(out, /*TODO non register returns*/
				"\tmov $0, %%eax\n"
				"\tret\n");
		break;
	case AstCond:
		break;
	case AstRef: {
		struct memloc ml;
		find_depth(cur, ast->val.ref->name, &ml);
		switch (ml.typ) {
		case MemStk:
			fprintf(out, "\tmov %ld(%%esp), %%eax\n", ml.val.dep);
			break;
		case MemExtrn:
			fprintf(out, ".extern %s\n", ast->val.ref->name);
		case MemStat:
			fprintf(out, "\tmov %s, %%eax\n", ast->val.ref->name);
			break;
		default:
			codegen_panic("Invalid mem type\n");
		}
		return;
	} break;
	case AstCall: {
		size_t i;
		for (i = ast->val.call.args.num_nodes; i > 0;) {
			codegen_node(&ast->val.call.args.nodes[--i]);
			fprintf(out, "\tpush %%eax\n");
		}
		codegen_node(ast->val.call.of);
		fprintf(out, "\tcall %%eax\n");
		if (ast->val.call.args.num_nodes) {
			fprintf(out,
					"\tadd $%lu, %%esp\n",
					(unsigned long)ast->val.call.args.num_nodes * ALIGN_SIZE); /*TODO bigger types*/
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
