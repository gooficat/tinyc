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
	union
	{
		integer_type_info i;
		struc_type_info	  s;
		ptr_type_info	  p;
	};
};

extern type INBUILT_TYPES[];
#define INBUILT_UCHAR INBUILT_TYPES[0]
#define INBUILT_SCHAR INBUILT_TYPES[1]
#define INBUILT_CHAR INBUILT_UCHAR

#define INBUILT_USHORT INBUILT_TYPES[2]
#define INBUILT_SSHORT INBUILT_TYPES[3]

#define INBUILT_ULONG INBUILT_TYPES[4]
#define INBUILT_SLONG INBUILT_TYPES[5]

#endif //!__TYPS__H__
