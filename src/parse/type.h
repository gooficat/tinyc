#ifndef __TYPE__H__
#define __TYPE__H__

#include "utils/vector.h"
#include <stddef.h>

typedef struct CType CType;
typedef struct CVar CVar; // TODO!!! ADDRESS ME

struct CType {
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
		CType *ptr;
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
			vec(CType) members;
		} struc;
		struct {
			vec(char *) keys;
		} enu;

		struct {
			CType *ret_type;
			vec(CVar) params;
		} func;
	};
};

typedef struct {
	char const *name;
	CType *type;
} CTypeDef;

#endif //!__TYPE__H__
