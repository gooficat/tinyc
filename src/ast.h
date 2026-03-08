#pragma once

typedef enum nodetype
{
	NoNode,
	VarDef,
	VarRef,
	Operation,
} nodetype;

typedef struct node node;

typedef enum vartype
{
	Number,
	String,
	Character,
} vartype;

struct vardef
{
	char *name;
	vartype type;
};

struct varref
{
	struct vardef *what;
};

struct operation
{
	node *left;
	node *right;
	char op;
};

struct funcdef
{
	char *name;
};

struct node
{
	nodetype type;
	union nodeu {
		struct varref varref;
		struct operation operation;
	} node;
};