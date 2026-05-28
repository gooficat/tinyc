#ifndef __DICT__H__
#define __DICT__H__

typedef enum {
	KW_AUTO,
	KW_REGISTER,
	KW_INT,
	KW_CHAR,
	KW_SHORT,
	KW_LONG,
	KW_FLOAT,
	KW_DOUBLE,
	KW_VOID,
	KW_STRUCT,
	KW_ENUM,
	KW_UNION,
	KW_CONST,
	KW_VOLATILE,
	KW_EXTERN,
	KW_STATIC,
	KW_IF,
	KW_WHILE,
	KW_DO,
	KW_FOR,
	KW_RETURN,
	KW_BREAK,
	KW_CONTINUE,
	KW_GOTO,
	KW_CASE,
	KW_SWITCH
} Keyword;

typedef enum {
	PN_SEMI,
	PN_COMM,
	PN_PARENL,
	PN_PARENR,
	PN_SQUARL,
	PN_SQUARR,
	PN_BRACEL,
	PN_BRACER,
	PN_COLON
} Punctuator;

typedef enum {
	OP_DOT,
	OP_ARR,
	OP_ADD_ASS,
	OP_SUB_ASS,
	OP_MUL_ASS,
	OP_DIV_ASS,
	OP_MOD_ASS,
	OP_AND_ASS,
	OP_OR_ASS,
	OP_XOR_ASS,
	OP_SHL_ASS,
	OP_SHR_ASS,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_AND,
	OP_OR,
	OP_XOR,
	OP_SHL,
	OP_SHR,
	OP_B_NOT,
	OP_EQU,
	OP_NEQ,
	OP_GEQ,
	OP_LEQ,
	OP_L_AND,
	OP_L_OR,
	OP_GREATER,
	OP_LESS,
	OP_NOT,
	OP_ASS
} Operator;

extern char const *const KEYWORDS[];
extern char const *const PUNCTUATORS[];
extern char const *const OPERATORS[];

#endif
