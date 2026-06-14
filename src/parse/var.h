#ifndef __VAR__H__
#define __VAR__H__

#include "lex/tok.h"
#include "type.h"

typedef enum {
	STORE_IMPLICIT = -1,
	STORE_AUTO = KW_AUTO,
	STORE_TYPEDEF,
	STORE_STATIC,
	STORE_EXTERN,
	STORE_INLINE,
} Storage;

typedef struct CVar {
	char const *name;
	Storage storage;
	CType *type;
} CVar;

typedef struct {
	char *name;
} Label;

#endif //!__VAR__H__
