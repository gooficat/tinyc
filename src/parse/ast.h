#ifndef __AST__H__
#define __AST__H__

#include "utils/vector.h"

typedef struct ASTNode ASTNode;

typedef struct {
	vec(ASTNode) children;
} ASTScope;

struct ASTNode {
	enum {
		AST_SCOPE,
	} type;
	union {
		ASTScope scope;
	};
};

#endif //!__AST__H__
