#ifndef __LEX__H__
#define __LEX__H__
#define CL_MAX_LINE 4096

void cl_open(char const *);
void cl_next(void);

enum TokenCat
{
};

enum Operator
{
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
};

enum Punctuator
{
	PN_COMMA,
	PN_SEMI,
	PN_
};

enum Keyword
{
	KW_AUTO,
};

#endif //!__LEX__H__
