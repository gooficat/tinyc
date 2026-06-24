#ifndef __TYPE__H__
#define __TYPE__H__

#include <stddef.h>

typedef struct type_s type_s;

typedef enum {
	TYPE_NONE,	// type is not deduced yet (used in parsing)
	TYPE_ERR,	// error types
	TYPE_INT,	// integer types
	TYPE_PTR,	// type points to another
	TYPE_FLOAT, // floating point (float, double, long double)
	TYPE_STRUC, // structured types
	TYPE_ENUM,	// enumerated types
} type_e;

typedef enum {
	SIGN_UNKNOWN,  // char has an unknown signedness. may also be used as a parsing helper
	SIGN_SIGNED,   // signed (can be negative)
	SIGN_UNSIGNED, // unsigned (positive only)
} sign_e;

typedef enum {
	INT_CHAR,
	INT_SHORT,
	INT_PLAIN, // the regular, undecorated integer type
	INT_LONG,
	INT_LONG_LONG,
} int_e;

typedef enum {
	FLT_PLAIN,
	FLT_DOUBLE,
	FLT_LONG_DOUBLE,
} float_e;

typedef struct {
	int_e type;
	sign_e sign;
} int_s;

struct type_s {
	type_e type; // category of type
	union {
		type_s *next; // either the underlying pointer type, or the list of structured elements (terminated by a type_none)
		int_s igr;
		float_e flt;
	} info;
};

#endif //!__TYPE__H__
