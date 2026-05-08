#include <stdio.h>

//////////////

struct PascalString
{
	char			  *data;
	unsigned long long length;
};

///////////////

enum CConstType
{
	CCONST_NONE,
	CCONST_INT,
	CCONST_FLOAT,
	CCONST_STRING,
};

union CConstVal
{
	long long			integer;
	long double			floating;
	struct PascalString string;
};

struct CConst
{
	enum CConstType type;
	union CConstVal val;
};

/////////////////

struct CType
{
	void *placeholder;
};

struct CSymbol
{
	struct CType *type;
};

////////////////

enum ASTNodeType
{
	ASTNODE_NONE,
	// ASTNODE_UNARY,
	// ASTNODE_BINARY,
	ASTNODE_FUNCTION,
	ASTNODE_CALL,
	ASTNODE_SYMBOL_REF,
	ASTNODE_ORDER,
	ASTNODE_CONDITIONAL,
	ASTNODE_SCOPE,
	ASTNODE_CONSTANT,
};

enum ASTOrderType
{
	ASTORDR_NONE,
	ASTORDR_RETURN,
	ASTORDR_GOTO,
	// ASTORDER_CONTINUE,
	// ASTORDER_BREAK,
};

enum ASTConditionalType
{
	ASTCOND_NONE,
	ASTCOND_IF,
	// ASTCOND_WHILE,
	// ASTCOND_DO_WHILE,
	// ASTCOND_FOR,
};

union ASTOrderValue
{
};

// struct ASTNodeUnary
// {

// };
// struct ASTNodeBinary
// {
// };

struct ASTNodeFunction
{
	struct CSymbol		*symbol;
	struct ASTNodeScope *scope;
};
struct ASTNodeCall
{
	struct ASTSymbol *symbol;
};
struct ASTNodeSymbolRef
{
	struct CSymbol *symbol;
};
struct ASTNodeOrder
{
	enum ASTOrderType	type;
	union ASTOrderValue value;
};
struct ASTNodeConditional
{
};
struct ASTNodeScope
{
};

union ASTNodeValue
{
	// struct ASTNodeUnary		  unary;
	// struct ASTNodeBinary	  binary;
	struct ASTNodeFunction	  function;
	struct ASTNodeCall		  call;
	struct ASTNodeSymbolRef	  symbol_ref;
	struct ASTNodeOrder		  order;
	struct ASTNodeConditional conditional;
	struct ASTNodeScope		  scope;
	struct CConst			  constant;
};

struct ASTNode
{
	enum ASTNodeType   type;
	union ASTNodeValue value;
};

////////////////

int main(void)
{
	puts("Hello");
	return 0;
}
