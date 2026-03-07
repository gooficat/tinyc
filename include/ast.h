#pragma once

#include "vector.h"
#include <stddef.h>
typedef enum
{
	AST_NODE_NONE,
} ASTNodeType;

typedef struct ASTNode ASTNode;

typedef struct
{
	char *name; // All variables are a "word" currently
} ASTVarDef;

typedef struct
{
	ASTNode *toWhat;
	ASTNode *value;
} ASTAssignment;

typedef struct
{
	ASTNode *left;
	ASTNode *right;
	char operation;
} ASTOperation;

typedef struct
{
	ASTVarDef *toWhat;
	ASTNode *value;
} ASTVarRef;

typedef struct
{
	struct vecStruct(ASTVarDef *, size_t) vars;
	struct vecStruct(ASTNode *, size_t) nodes;
} ASTScope;

typedef struct
{
	ASTScope scope;
	size_t numArgs;
} ASTFunction;

typedef struct
{
	ASTFunction *toWhat;
	struct vecStruct(ASTNode *, size_t) args;
} ASTCall;

typedef struct
{
	ASTScope scope;
	struct vecStruct(ASTFunction *, size_t) functions;
} ASTGlobal;

struct ASTNode
{
	ASTNodeType type;
	union {
		ASTVarRef varRef;
		ASTAssignment assignment;
		ASTOperation operation;
		ASTScope scope;
		ASTFunction function;
		ASTCall call;
	};
};
