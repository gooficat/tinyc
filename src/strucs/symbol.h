#ifndef __SYMBOL__H__
#define __SYMBOL__H__

#include "types.h"
#include "utils/pstr.h"
#include <stddef.h>

struct c_var {
	struct c_type *type;
};

struct c_label {
	char *name;
};

#endif //!__SYMBOL__H__
