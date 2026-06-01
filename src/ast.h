#ifndef __AST__H__
#define __AST__H__

#include "da.h"
#include "lex.h"
#include "val.h"

typedef struct {
	char const *name;
} CSymbol;

typedef enum ASTType {
	AST_NONE,
	AST_SCOPE,
	AST_BINOP,
	AST_UNOP,
	AST_CALL,
	AST_FUNC,
	AST_ORDER,
	AST_CONST,
	AST_REF
} ASTType;
typedef struct ASTScope {
	struct ASTNode	*members;
	CSymbol			*symbols;
	struct ASTScope *parent;
} ASTScope;

typedef enum {
	ORDER_NONE,
	ORDER_RETURN,
	ORDER_BREAK,
	ORDER_CONTINUE,
	ORDER_GOTO
} OrderType;

typedef struct ASTNode {
	ASTType type;
	union {
		ASTScope scope;
		struct {
			struct ASTNode *l, *r;
		} binop;
		struct {
			struct ASTNode *n;
		} unop;
		struct {
			struct ASTNode *of;
		} call;
		struct {
			CSymbol		   *symbol;
			struct ASTScope body;
		} func;
		struct {
			OrderType type;
			union {
				struct ASTNode *expr;
				CSymbol		   *label;
			} val;
		} order;
		CConst	*constant;
		CSymbol *ref;
	} val;
} ASTNode;

void ParseTree(ASTNode *node, Lexer *lexer);

#endif
