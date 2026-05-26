#ifndef __VAL__H__
#define __VAL__H__

#include <stdint.h>

typedef enum {
	TYPE_NONE,
	TYPE_INTEGER,
	TYPE_FLOATING,
	TYPE_STRUCTURED
} CTypeType;

typedef struct {
	CTypeType type;
} CType;

typedef struct {
	CType *type;
} CSymbol;

typedef struct {
	CType *type;
	union {
		int64_t		i;
		long double f;
		char const *s;
	} val;
} CConst;

#endif
