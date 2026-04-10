#ifndef __TYPS__H__
#define __TYPS__H__

#include "vectr.h"
typedef enum
{
	TYPE_ERR,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_POINTER,
	TYPE_STRUCTURED,
	TYPE_ENUMERATED,
} type_cat;
typedef struct type type;
vec_type(type);
typedef struct
{
	type *und_type;
} ptr_type_info;

typedef struct
{
	unsigned char is_union;
	vec_type	  members;
} struc_type_info;

typedef struct
{
	unsigned char is_signed;
} integer_type_info;

struct type
{
	type_cat		   cat;
	unsigned long long size;
	void			  *info;
};

#endif //!__TYPS__H__
