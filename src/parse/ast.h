#ifndef __AST__H__
#define __AST__H__

#include "lex/tok.h"
#include "utils/vector.h"
#include "var.h"

typedef struct ASTNode ASTNode;
typedef struct ASTScope ASTScope;

struct ASTScope {
	vec(ASTNode) children;
	vec(Var) vars;
	vec(TypeDef) types;
	vec(TypeDef) tagged;
	vec(Label) labels;
	ASTScope *parent;
};

typedef struct {
	ASTNode *of, *with;
} ASTCall;

typedef enum {
	ORDER_RETURN = KW_RETURN,
	ORDER_BREAK,
	ORDER_CONTINUE,
	ORDER_GOTO,
} OrderType;

typedef struct {
	OrderType type;
	union {
		ASTNode *expr;
		Label *label;
	};
} ASTOrder;

typedef struct {
	Type *to;
	ASTNode *node;
} ASTCast;

typedef struct {
	Var *var;
	ASTScope body;
} ASTFunc;

struct ASTNode {
	enum {
		AST_SCOPE,
		AST_VREF,
		AST_CHAIN,
		AST_CALL,
		AST_ORDER,
		AST_CNST,
		AST_CAST,
		AST_FUNC,
	} type;
	union {
		ASTScope scope;
		Var *vref;
		vec(ASTNode) chain;
		ASTCall call;
		ASTOrder order;
		Const cnst;
		ASTCast cast;
		ASTFunc func;
	};
};

extern ASTNode tree;

#endif //!__AST__H__
