#include "main.h"
#include <stdlib.h>

struct csym_table *tbl = NULL;

void gen_expr(void);
void gen_stmt(void);

void enter_scope(void)
{
	struct csym_table *old = tbl;
	tbl = malloc(sizeof *tbl);
	tbl->prev = old;
	tbl->syms.els = NULL;
	tbl->syms.len = 0;
}
void exit_scope(void)
{
	struct csym_table *old = tbl;
	free(tbl->syms.els);
	tbl = tbl->prev;
	free(old);
}

static void gen_compound(void)
{
}

static void gen_vardef(void)
{
}

static void gen_parenthesized(void)
{
	lex_next();

	if (tok.type <= TOK_FLOAT) // TODO
	{
		gen_vardef();
	}
	else
	{
		gen_expr();
		if (tok.type != TOK_PAREN_R)
		{
			// error
			exit(EXIT_FAILURE);
		}
		lex_next();
	}
}

void postfix_expr(void) // TODO!!!!! result location
{
}

void gen_prefix_oper(void)
{
	int oper = tok.type;
	lex_next();
}

void perform_op(int op)
{
	switch (op)
	{
	case TOK_PLUS:
		printf("\tadd %%rax, %%rbx\n");
		break;
	case TOK_MINUS:
		printf("\tsub %%rax, %%rbx\n");
		break;
	case TOK_STAR:
		printf("\timul %%rbx\n");
		break;
	case TOK_SLASH:
		printf("\tidiv %%rbx\n");
		break;
	case TOK_MOD:
		printf("\tidiv %%rbx\n"
			   "\tmov %%rax, %%rdx\n");
		break;
	case TOK_AMP:
		printf("\tand %%rax, %%rbx\n");
		break;
	case TOK_BAR:
		printf("\tor %%rax, %%rbx\n");
		break;
	case TOK_XOR:
		printf("\txor %%rax, %%rbx\n");
		break;
	}
}

void oper_expr(void)
{
	int op = tok.type;
	lex_next();
	// TODO IF RESULT IS STACK THEN DONT DO THIS
	printf("\tpush %%rax\n");
	gen_expr();
	if (tok.type == TOK_EQU)
	{
		int op2 = tok.type;
		lex_next();
		printf("\tpush %%rax\n");
		gen_expr();
		printf("\tpop %%rbx\n");
		perform_op(op2);
	}
	printf("\tpop %%rbx\n"); // TODO this isnt quite what we want
}

void gen_expr(void)
{
	// lval:
	if (tok.type == TOK_IDEN)
	{
		size_t idx = tok.val;
		lex_next();
		if (tok.type == TOK_EQU)
		{
			struct mem_desc md;
			lex_next();
			gen_expr();
			// TODO ACCUM VS STACK, DISAMBIGUATION TOO
			find_sym(syms.els[idx], tbl, &md);
			if (md.type == MEM_STT)
			{
				printf("\tmov %s, %%rax\n", syms.els[idx]);
			}
			else if (md.type == MEM_STK)
			{
				printf("\tmov %llu, %%rax\n", md.val);
			}
			else
			{
				// TODO
				exit(EXIT_FAILURE);
			}
		}
	}
	else if (tok.type == TOK_STAR)
	{
		lex_next();
		gen_expr();
		if (tok.type == TOK_EQU)
		{
			lex_next();
			printf("push %%rax\n");
			gen_expr();
			printf("pop %%rbx\n");
			printf("\tmov (%%rbx), %%rax\n");
		}
	}
	// rval:
	else if (tok.type == TOK_BRACE_L)
	{
		gen_compound();
	}
	else if (tok.type == TOK_PAREN_L)
	{
		gen_parenthesized();
	}
	else if (tok.type >= TOK_PLUS && tok.type <= TOK_EQU)
	{
		gen_prefix_oper();
	}
	else
	{
		exit(EXIT_FAILURE);
	}
	postfix_expr();

	if (tok.type >= TOK_PLUS && tok.type <= TOK_EQU)
	{
		oper_expr();
	}
}

void gen_def(void)
{
	int type = tok.type;
	lex_next();
	char *name = tok.val; // TODO
	lex_next();
	if (tok.type == TOK_PAREN_L)
	{
		struct sym_list syms;
		syms.els = NULL;
		syms.len = 0;
		while (tok.type != TOK_PAREN_R)
		{
			struct csym sym;
			sym.type = NULL;
			sym.name = tok.val;
		}
	}
}

void gen_order(void)
{
}

void gen_stmt(void)
{
	if (tok.type >= TOK_INT && tok.type <= TOK_FLOAT)
	{
		gen_def();
	}
	else if (tok.type >= TOK_RETURN && tok.type <= TOK_GOTO)
	{
		gen_order();
	}
	else if (tok.type != TOK_SEMI)
	{
		gen_expr();
	}
	else
	{
		lex_next();
	}
}

void gen_code(void)
{
	printf(".code64\n.text\n");
	enter_scope();
	while (tok.type != TOK_EOF)
	{
		gen_stmt();
	}
	exit_scope();
}
