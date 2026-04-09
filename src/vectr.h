#ifndef __VECTR__H__
#define __VECTR__H__
#include <stdlib.h>

#define vec_struct(t)                \
	struct vec_##t                   \
	{                                \
		t				  *data;     \
		unsigned long long len, cap; \
	}
#define vec_type(t) typedef vec_struct(t) vec_##t##_s
#define vec_init(v)                          \
	do                                       \
	{                                        \
		(v).data = malloc(sizeof *(v).data); \
		(v).len	 = 0;                        \
		(v).cap	 = 1;                        \
	} while (0)
#define vec_update(v) (v).data = realloc((v).data, (v).cap * sizeof *(v).data)
#define vec_trim(v) (v).data = realloc((v).data, (v).len * sizeof *(v).data)
#define vec_push(v, p)             \
	do                             \
	{                              \
		if ((v).len >= (v).cap)    \
		{                          \
			(v).cap *= 2;          \
			vec_update(v);         \
		}                          \
		(v).data[(v).len++] = (p); \
	} while (0)
#define vec_pop(v)                   \
	do                               \
	{                                \
		if (--(v).len < (v).cap / 2) \
		{                            \
			(v).cap /= 2;            \
			vec_update(v);           \
		}                            \
	} while (0)

#endif //!__VECTR__H__
