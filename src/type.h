#ifndef __TYPE__H__
#define __TYPE__H__

#include "tiny_c_libs/vector.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct type_s type_s;

typedef enum {
	TYPE_NONE,	// type is not deduced yet (used in parsing)
	TYPE_ERR,	// error types
	TYPE_INT,	// integer types
	TYPE_VOID,	// void type
	TYPE_PTR,	// type points to another
	TYPE_FLOAT, // floating point (float, double, long double)
	TYPE_STRUC, // structured types
	TYPE_ENUM,	// enumerated types
	TYPE_FUNC,	// a function
} type_e;

typedef enum {
	INT_CHAR,
	INT_SHORT,
	INT_PLAIN, // the regular, undecorated integer type
	INT_LONG,
	INT_LONG_LONG,
} int_e;

typedef enum {
	SIGN_UNKNOWN,  // char has an unknown signedness. may also be used as a parsing helper
	SIGN_SIGNED,   // signed (can be negative)
	SIGN_UNSIGNED, // unsigned (positive only)
} sign_e;

typedef struct {
	int_e type;
	sign_e sign;
} int_s;

typedef enum {
	FLT_PLAIN,
	FLT_DOUBLE,
	FLT_LONG_DOUBLE,
} float_e;

typedef struct {
	type_s *under;
	size_t size;
} array_s;

typedef struct {
	vec(type_s) mems; // members
	bool unified;
} struct_s;

typedef struct {
	type_s *ret_typ;
	vec(type_s) params;
} func_s;

typedef union {
	type_s *ptr;		// underlying type of a pointer type
	vec(char *) e_vals; // enumerated values
	struct_s stc;
	int_s igr;
	float_e flt;
	array_s arr;
	func_s fun;
} type_u;

struct type_s {
	type_e type; // category of type
	type_u info;
};

#endif //!__TYPE__H__
