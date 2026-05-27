#ifndef __VAL__H__
#define __VAL__H__

#include <stdbool.h>
#include <stdint.h>
#include <vcruntime.h>

typedef enum {
	TYPE_NONE,
	TYPE_INTEGER,
	TYPE_FLOATING,
	TYPE_POINTER,
	TYPE_STRUCTURED
} CTypeType;

typedef struct {
	CTypeType type;
	union {
		struct {
			bool	is_signed;
			uint8_t width;
		} i;
		struct {
			uint8_t width;
		} f;
		struct {
			struct CType *underlying;
		} p;
		struct {
			struct CType *members;
			size_t		  num_members;
		} s;
	} val;
	bool		  is_function;
	bool		  is_const;
	struct CType *args;
	size_t		  num_args;
} CType;

typedef struct {
	char const *name;
	CType	   *type;
} CVariable;

typedef struct {
	CType *type;
	union {
		int64_t		i;
		long double f;
		char const *s;
	} val;
} CConst;

#endif
