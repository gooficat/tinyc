#include <stdio.h>
#include <vcruntime.h>

//////////////

#define safe_macro(m, ...) \
	do                     \
	{                      \
		m(__VA_ARGS__)     \
	} while (0)

//////////////

#define vec_struct_body(type)    \
	{                            \
		type		 *data;      \
		unsigned long size, cap; \
	}

#define Mvec_init(vec)                   \
	vec.data = malloc(sizeof *vec.data); \
	vec.size = 0;                        \
	vec.cap	 = 1;
#define vec_init(vec) safe_macro(Mvec_init, (vec))

#define vec_update(vec) (vec).data = realloc((vec).data, (vec).size * sizeof *(vec).data)

#define Mvec_push(vec, new)  \
	if (vec.size >= vec.cap) \
	{                        \
		vec.cap *= 2;        \
		vec_update(vec);     \
	}                        \
	vec.data[vec.size++] = new;
#define vec_push(vec, new) safe_macro(Mvec_push, (vec), (new))

#define vec_type(type) typedef struct vec_struct(type) Vec_##type;

//////////////

struct PascalString
{
	char		 *data;
	unsigned long length;
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
	long				integer;
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

struct Vec_ASTNode vec_struct_body(struct ASTNode);

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

union ASTConditionalValue
{
	struct ASTNode *expr;
	// struct ASTNode* triple_expr[3]; // for loops
};

union ASTOrderValue
{
	struct ASTNode *expr;
	struct CSymbol *symbol;
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
	enum ASTConditionalType	   type;
	struct ASTConditionalValue value;
};

struct ASTNodeScope
{
	struct Vec_ASTNode body;
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

void GenAST(struct ASTNode *node);

////////////////

int main(void)
{
	puts("Hello");
	return 0;
}
