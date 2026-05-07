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

enum ASTNodeType
{
	ASTNODE_NONE,
	ASTNODE_UNARY,
	ASTNODE_BINARY,
	ASTNODE_FUNCTION,
	ASTNODE_CALL,
	ASTNODE_SYMBOL_REF,
	ASTNODE_ORDER,
	ASTNODE_CONDITIONAL,
	ASTNODE_SCOPE,
};

struct ASTNode
{
	enum ASTNodeType type;
};

////////////////
int main(void)
{
	puts("Hello");
	return 0;
}
