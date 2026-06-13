#ifndef __TYPES__H__
#define __TYPES__H__

#include "utils/vector.h"
#include <stddef.h>
#include <stdint.h>

enum c_type_type {
	C_TYPE_NONE,
	C_TYPE_ERR,
	C_TYPE_INT,
	C_TYPE_FLT,
	C_TYPE_STRUC,
	C_TYPE_ARRAY,
	C_TYPE_ENUM,
	C_TYPE_CHAR,
	C_TYPE_PTR,
};

struct c_type {
	enum c_type_type type;
	size_t width;
	vec(struct c_type) params;
	void *data;
};

#endif //!__TYPES__H__
