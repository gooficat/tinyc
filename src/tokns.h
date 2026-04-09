#ifndef __TOKNS__H__
#define __TOKNS__H__

typedef enum
{
	KWD_AUTO,
	KWD_INT,
	KWD_CHAR,
	KWD_SHORT,
	KWD_SIGNED,
	KWD_UNSIGNED,
	KWD_LONG,
	KWD_FLOAT,
	KWD_DOUBLE,
	KWD_VOID,
	KWD_CONST,
	KWD_STATIC,
	KWD_EXTERN,
	KWD_RETURN,
	KWD_BREAK,
	KWD_CONTINUE,
	KWD_IF,
	KWD_WHILE,
	KWD_DO,
	KWD_FOR,
} keyword_e;

#define NUM_KEYWORDS (KWD_FOR + 1)

typedef enum
{
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
	OP_LESS,
	OP_MORE,
	OP_L_NOT,
	OP_NOT,
	OP_L_OR,
	OP_L_AND,
	OP_L_EQU,
	OP_L_NEQ,
	OP_L_LEQ,
	OP_L_GEQ,
	OP_A_ADD,
	OP_A_SUB,
	OP_A_MUL,
	OP_A_DIV,
	OP_A_MOD,
	OP_A_AND,
	OP_A_OR,
	OP_A_XOR,
	OP_A_SHL,
	OP_A_SHR,
	OP_INC,
	OP_DEC,
	OP_EQU,
	OP_ARROW,
} operator_e;

#define NUM_OPERATORS (OP_ARROW + 1)

typedef enum
{
	PNC_PAREN_L,
	PNC_PAREN_R,
	PNC_CURLY_L,
	PNC_CURLY_R,
	PNC_ANGLE_L,
	PNC_ANGLE_R,
	PNC_DOT_OP,
	PNC_SEMI,
	PNC_COLON,
} punctuator_e;

#define NUM_PUNCTUATORS (PNC_COLON + 1)

extern const char *KEYWORDS[];
extern const char *OPERATORS[];
extern const char *PUNCTUATORS[];

#endif //!__TOKNS__H__
