#ifndef __TYPE__H__
#define __TYPE__H__

#include "utils/vector.h"
#include <stddef.h>

typedef struct Type Type;
struct Type {
	enum {
		TYPE_ERR,
		TYPE_VOID,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRUC,
		TYPE_ENUM,
		TYPE_PTR,
		TYPE_FUNC,
	} type;
	union {
		Type *ptr;
		struct {
			enum {
				INT_CHAR,
				INT_SHORT,
				INT_INT,
				INT_LONG,
				INT_LONG_LONG,
			} precision;
			bool is_signed;
		} igr;
		struct {
			enum {
				FLT_FLOAT,
				FLT_DOUBLE,
				FLT_LONG_DOUBLE,
			} precision;
		} flt;
		struct {
			vec(Type) members;
		} struc;
		struct {
			vec(char *) keys;
		} enu;

		struct {
			Type *ret_type;
			vec(Type) params;
		} func;
	};
};

#endif //!__TYPE__H__
