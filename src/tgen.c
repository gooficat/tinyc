#include "lexer.h"
#include "nods.h"
#include "tokns.h"
#include "types.h"
#include "vectr.h"
#include <stdio.h>
#include <stdlib.h>

void gen_scope(ast_scope *sc, tok_strm *ts);
void handle_kwd(ast_scope *sc, tok_strm *ts);
void handle_node(ast_scope *sc, tok_strm *ts);
void gen_node(ast_node *n, ast_scope *sc, tok_strm *ts);
void init_scope(ast_scope *sc, ast_scope *parent);
void fill_scope(ast_scope *sc, tok_strm *ts);
void types_match(type *a, type *b);
void find_type(ast_scope *sc, type *t);

void handle_stmt(ast_scope *sc, tok_strm *ts)
{
	switch (ts->tok.typ)
	{
	// case TOK_EOF:
	case TOK_CONST:
		// this is not allowed at top level
		fprintf(stderr, "ERROR! Numeric literals may not appear at the top level of a statement");
		break;
	case TOK_KEYW:
		handle_kwd(sc, ts);
		break;
	case TOK_IDENT:
	case TOK_OPER:
	case TOK_PUNC:
		handle_node(sc, ts);
	// case TOK_STRLIT:
	default: // EOF is functionally impossible so strlit is the intent here
			 // strings at top level are also wrong (for now (terry davis hint hint))
		fprintf(stderr, "ERROR! String literals may not appear at the top level of a statement");
		break;
	}
}

void gen_scope(ast_scope *sc, tok_strm *ts)
{
}

void handle_kwd(ast_scope *sc, tok_strm *ts)
{
}

void handle_node(ast_scope *sc, tok_strm *ts)
{
}

void gen_node(ast_node *n, ast_scope *sc, tok_strm *ts)
{
	if (ts->tok.typ == TOK_CONST)
	{
		ast_literal *lit	   = malloc(sizeof(*lit));
		lit->i.val			   = ts->tok.num;
		lit->cat			   = NUMERIC_LITERAL;
		lit->type			   = malloc(sizeof *lit->type);
		lit->type->cat		   = TYPE_INT;
		lit->type->size		   = sizeof(int);
		lit->type->i.is_signed = 1;
		// for now it's all 4 byte signed integers. suffixes like llu and whatnot can come later (TODO)
		n->node = lit;
		return;
	}
	{
		ast_literal *lit = malloc(sizeof(*lit));
		n->type			 = AST_LITERAL;
		// lit->type
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
