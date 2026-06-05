#ifndef __VAL__H__
#define __VAL__H__

struct cnst {
  enum { ConstNone, ConstInt, ConstFloat, ConstString } typ;
  union {
    long i;
    long double f;
    char const *s;
  } val;
};

#endif
