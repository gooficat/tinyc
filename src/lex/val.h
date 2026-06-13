#ifndef __VAL__H__
#define __VAL__H__

typedef struct {
	enum {
		CONST_INT,
		CONST_FLT,
		CONST_STR,
	} type;
	union {
		long long i;
		long double f;
		char *s;
	};
} Const;

#endif //!__VAL__H__
