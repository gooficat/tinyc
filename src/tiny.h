#ifndef __TINY__H__
#define __TINY__H__

#include "type.h"
#include <stdint.h>

typedef enum {
	C_VAL_INT,
	C_VAL_FLOAT,
	C_VAL_STRING,
} c_val_e;
typedef union {
	intmax_t igr;
	long double flt;
	char *str;
} c_val_u;
typedef struct {
	c_val_e type;
	c_val_u val;
} c_val_s;

typedef enum {
	STORE_IMPLICIT,
	STORE_AUTO,
	STORE_EXTERN,
	STORE_STATIC,
	STORE_TYPEDEF,
} storag_e;
typedef struct {
	char *name;
	type_s *type;
	storag_e storage;
} c_sym_s;

#endif //!__TINY__H__
