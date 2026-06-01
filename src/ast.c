#include "ast.h"
#include "da.h"
#include "dict.h"
#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void HandleStmt(Lexer *lexer, ASTScope *scope);
static void GenNode(ASTNode *node, Lexer *lexer, ASTScope *scope);

static CSymbol *FindSymbol(char const *const name, ASTScope *scope) {
	size_t i;
	for (i = 0; i < dalen(scope->symbols); ++i) {
		if (!strcmp(name, scope->symbols[i].name)) {
			return &scope->symbols[i];
		}
	}
	if (scope->parent) {
		return FindSymbol(name, scope->parent);
	}
	return NULL;
}

static void GenScope(ASTNode *node, Lexer *lexer, ASTScope *scope) {
	while (lexer->token.type != TOK_PUNC || lexer->token.data.pn != PN_BRACER) {
		HandleStmt(lexer, &node->val.scope);
	}
}

static void GenNode(ASTNode *node, Lexer *lexer, ASTScope *scope) {
	switch (lexer->token.type) {
	case TOK_IDEN:
		node->type	  = AST_REF;
		node->val.ref = FindSymbol(lexer->token.data.id, scope);
		break;
	case TOK_KEYW:
		if (lexer->token.data.kw >= KW_IF && lexer->token.data.kw <= KW_FOR) {
			/*conditional*/
			return;
		}
		if (lexer->token.data.kw >= KW_RETURN && lexer->token.data.kw <= KW_GOTO) {
			return;
		}
		switch (lexer->token.data.kw) {
		case KW_IF:
		case KW_WHILE:
		case KW_DO:
		case KW_FOR:
		case KW_RETURN:
		case KW_BREAK:
		case KW_CONTINUE:
		case KW_GOTO:
		case KW_CASE:
		case KW_SWITCH:
			break;
		default:
			node->type = AST_NONE;
		}
		break;
	case TOK_OPER:
	case TOK_PUNC:
	case TOK_CNST:
	default:
		node->type = AST_NONE;
		return;
	}
}

static void HandleDef(Lexer *lexer, ASTScope *scope) {
	if (lexer->token.data.kw == KW_INT) {
		Lexer_Next(lexer);
	} else {
		fprintf(stderr, "Unimplemented\n");
		exit(EXIT_FAILURE);
	}
}

static void HandleStmt(Lexer *lexer, ASTScope *scope) {
	if (lexer->token.type == TOK_KEYW) {
		if (lexer->token.data.kw > KW_STATIC) {
			ASTNode node;
			GenNode(&node, lexer, scope);
			dapsh(scope->members, &node);
			return;
		}
		HandleDef(lexer, scope);
		return;
	}
}

void ParseTree(ASTNode *node, Lexer *lexer) {
	node->type				= AST_SCOPE;
	node->val.scope.members = damk(ASTNode);
	node->val.scope.symbols = damk(CSymbol);
	while (lexer->token.type != TOK_NONE) {
		HandleStmt(lexer, NULL);
	}
}
