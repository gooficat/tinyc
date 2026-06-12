#ifndef __VALUE__H__
#define __VALUE__H__

#include "utils/vector.h"
#include <stdint.h>

enum c_const_type {
	C_CONST_COMP,
	C_CONST_INT,
	C_CONST_FLOAT,
};

struct c_const {
	enum c_const_type type;
	union {
		intmax_t i;
		long double f;
		vec(struct c_const) c;
	};
};

vec(struct c_const) CONSTANTS;

#endif //!__VALUE__H__
