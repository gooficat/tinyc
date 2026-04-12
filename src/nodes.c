#include "nodes.h"
#include "lexer.h"
#include "tokns.h"
#include "types.h"
#include "vectr.h"
#include <stdio.h>
#include <stdlib.h>

void gen_scope(ast_scope *sc, tok_strm *ts);
void handle_kwd(ast_scope *sc, tok_strm *ts);
void handle_node(ast_scope *sc, tok_strm *ts);
void handle_type(ast_scope *sc, tok_strm *ts);
void gen_node(ast_node *n, ast_scope *sc, tok_strm *ts);
void init_scope(ast_scope *sc, ast_scope *parent);
void fill_scope(ast_scope *sc, tok_strm *ts);
void types_match(type *a, type *b);
void find_type(ast_scope *sc, type *t);

void handle_stmt(ast_scope *sc, tok_strm *ts)
{
	if (ts->tok.typ == TOK_KEYW)
	{
		handle_kwd(sc, ts);
		return;
	}
	handle_node(sc, ts);
}

void gen_scope(ast_scope *sc, tok_strm *ts)
{
}

void parse_params(vec_type *t, tok_strm *ts)
{
	while (ts->tok.typ != TOK_PUNC || ts->tok.num != PNC_PAREN_R)
	{
		// TODO
	}
}

void handle_type(ast_scope *sc, tok_strm *ts)
{
	ast_sym s;
	/// TODO (only ints work rn) (need signed, unsinged, long ,char , short, const and especcialy POINTERS)
	tok_strm__next(ts);
	s.type = &INBUILT_SLONG;
	if (ts->tok.typ != TOK_IDENT)
	{
		fprintf(stderr, "Wrong node succeeding type!\n");
		exit(EXIT_FAILURE);
	}
	s.name = ts->tok.str;
	tok_strm__next(ts);
	if (ts->tok.typ == TOK_PUNC)
	{
		if (ts->tok.num == PNC_PAREN_L)
		{
			tok_strm__next(ts);
			vec_init(s.args);
			parse_params(&s.args, ts);
			tok_strm__next(ts);
		}
		else if (ts->tok.num == PNC_ANGLE_L)
		{
			// ARRAYS (todo) (oh the pain)
		}
	}
}

void handle_ordr(ast_scope *sc, tok_strm *ts)
{
	ast_node   n;
	ast_order *o = malloc(sizeof *o);
	n.type		 = AST_ORDER;
	switch (ts->tok.num)
	{
	case KWD_CONTINUE:
		o->cat = AST_ORDER_CONTINUE;
		break;
	case KWD_BREAK:
		o->cat = AST_ORDER_CONTINUE;
		break;
	case KWD_RETURN:
		o->cat = AST_ORDER_CONTINUE;
		break;
	// case KWD_GOTO:
	default:
		fputs("Unknown order", stderr);
		exit(EXIT_FAILURE);
	}
	n.type = AST_ORDER;
	n.node = o;
	vec_push(sc->nodes, n);
}

void handle_kwd(ast_scope *sc, tok_strm *ts)
{
	if (ts->tok.num <= KWD_EXTERN)
	{
		handle_type(sc, ts);
		return;
	}
	if (ts->tok.num <= KWD_CONTINUE)
	{
		if (sc->parent == NULL)
		{
			fputs("Error! Orders may not appear at the top level scope!", stderr);
			exit(EXIT_FAILURE);
		}
		handle_ordr(sc, ts);
	}
}

void handle_node(ast_scope *sc, tok_strm *ts)
{
	ast_node n;
	gen_node(&n, sc, ts);
	while (ts->tok.typ == TOK_OPER)
	{
		ast_node *r = malloc(sizeof *r);
		gen_node(r, sc, ts);
	}
	vec_push(sc->nodes, n);
}

type *pointing_type(type *to)
{
	type *t		  = malloc(sizeof *t);
	t->cat		  = TYPE_POINTER;
	t->size		  = 8;
	t->p.und_type = to;
	return t;
}

void gen_node(ast_node *n, ast_scope *sc, tok_strm *ts)
{
	if (ts->tok.typ == TOK_CONST)
	{
		ast_literal *lit;
		lit					   = malloc(sizeof(*lit));
		lit->i.val			   = ts->tok.num;
		lit->cat			   = NUMERIC_LITERAL;
		lit->type			   = malloc(sizeof *lit->type);
		lit->type->cat		   = TYPE_INT;
		lit->type->size		   = sizeof(int);
		lit->type->i.is_signed = 1;
		// for now it's all 4 byte signed integers. suffixes like llu and whatnot can come later (TODO)
		return;
	}
	if (ts->tok.typ == TOK_STRLIT)
	{
		ast_literal *lit;
		lit		   = malloc(sizeof(*lit));
		n->type	   = AST_LITERAL;
		lit->type  = pointing_type(&INBUILT_CHAR);
		lit->s.val = ts->tok.str;
		n->node	   = lit;
		return;
	}
}

void init_scope(ast_scope *sc, ast_scope *parent)
{
	vec_init(sc->nodes);
	vec_init(sc->syms);
	vec_init(sc->types);
	sc->parent = parent;
}

void fill_scope(ast_scope *sc, tok_strm *ts)
{
	while (ts->tok.typ != TOK_PUNC || ts->tok.num != PNC_CURLY_R)
	{
		handle_stmt(sc, ts);
	}
}

void ast_gen(ast_scope *sc, tok_strm *ts)
{
	while (ts->tok.typ != TOK_EOF)
	{
		handle_stmt(sc, ts);
	}
}
