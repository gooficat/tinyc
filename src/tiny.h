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

#endif //!__TINY__H__
