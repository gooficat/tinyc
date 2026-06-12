#ifndef __VALUE__H__
#define __VALUE__H__

#include "parse/ctx.h"
#include "utils/pstr.h"
#include "utils/vector.h"
#include <stddef.h>
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
		struct pstr s;
	};
};

size_t gen_constant(struct parse_ctx *ctx);

#endif //!__VALUE__H__
