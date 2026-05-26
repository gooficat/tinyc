#ifndef __AST__H__
#define __AST__H__

#include "val.h"
#include "vec.h"

typedef enum ASTType {
	AST_NONE,
	AST_SCOPE,
	AST_BINOP,
	AST_UNOP,
	AST_CALL,
	AST_FUNC,
	AST_ORDER,
	AST_CONST
} ASTType;

typedef struct vec_body(struct ASTNode) Vec_ASTNode;

struct ASTScope {
	Vec_ASTNode body;
};

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
		struct ASTScope scope;
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
		CConst *constant;
	} val;
} ASTNode;

void ParseTree(ASTNode *node);

#endif
