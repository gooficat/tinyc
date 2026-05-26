#ifndef __VEC__H__
#define __VEC__H__

#define vec_body(t)             \
	{                           \
		t			 *val;      \
		unsigned long len, cap; \
	}
#define vec_push(v, n)                                             \
	do {                                                           \
		if ((v).len >= (v).cap) {                                  \
			(v).cap *= 2;                                          \
			(v).val = realloc((v).val, (v).cap * sizeof *(v).val); \
		}                                                          \
		(v).val[(v).len++] = (n);                                  \
	} while (0)

#endif
