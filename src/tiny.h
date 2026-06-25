#ifndef __TINY__H__
#define __TINY__H__

#include <stdint.h>

typedef enum {
	C_VAL_INT,
	C_VAL_FLT,
	C_VAL_STR,
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
	C_SYM_TDEF,
	C_SYM_VAR,
} c_sym_e;
typedef union {
	void *placeholder;
} c_sym_u;
typedef struct {
	char *name;
	c_sym_e typ;
	c_sym_u info;
} c_sym_s;

#endif //!__TINY__H__
