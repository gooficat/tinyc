#ifndef __VAR__H__
#define __VAR__H__

#include "type.h"
typedef struct {
	char const *name;
	enum {
		VAR_STATIC,
		VAR_EXTRN,
		VAR_AUTO,
		VAR_INLINE,
	} storage;
	Type *type;
} Var;

typedef struct {
	char *name;
} Label;

#endif //!__VAR__H__
