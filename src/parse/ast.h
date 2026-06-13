#ifndef __AST__H__
#define __AST__H__

#include "utils/vector.h"
#include "var.h"

typedef struct ASTNode ASTNode;
typedef struct ASTScope ASTScope;

struct ASTScope {
	vec(ASTNode) children;
	vec(Var) vars;
	ASTScope *parent;
};

typedef struct {
	ASTNode *of, *with;
} ASTCall;

typedef struct {
	int order_type;
	union {
		ASTNode *sym;
		Label *label;
	};
} ASTOrder;

struct ASTNode {
	enum {
		AST_SCOPE,
		AST_VREF,
		AST_CHAIN,
		AST_CALL,
		AST_ORDER,
	} type;
	union {
		ASTScope scope;
		vec(ASTNode) chain;
		ASTCall call;
		Var *ref;
		ASTOrder order;
	};
};

#endif //!__AST__H__
