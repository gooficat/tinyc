#ifndef __TYPS__H__
#define __TYPS__H__

typedef enum
{
	TYPE_ERR,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRUCTURED,
} type_cat;

typedef struct
{
	type_cat		   cat;
	unsigned long long size;
} type;

#endif //!__TYPS__H__
