#include "types.h"
#include "utils/hash.h"

const struct c_type TYPES[] = {
	{C_TYPE_INT, 1, (void *)true},	 {C_TYPE_INT, 2, (void *)true},
	{C_TYPE_INT, 4, (void *)true},	 {C_TYPE_INT, 8, (void *)true},

	{C_TYPE_INT, 1, (void *)false},	 {C_TYPE_INT, 2, (void *)false},
	{C_TYPE_INT, 4, (void *)false},	 {C_TYPE_INT, 8, (void *)false},

	{C_TYPE_CHAR, 1, (void *)false},

	{C_TYPE_FLT, 4, NULL},			 {C_TYPE_FLT, 8, NULL},
};
