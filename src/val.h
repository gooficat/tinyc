#ifndef __VAL__H__
#define __VAL__H__

#include <stdbool.h>
#include <vcruntime.h>
struct cnst {
  enum { ConstNone, ConstInt, ConstFloat, ConstString } typ;
  union {
    long i;
    long double f;
    char const *s;
  } val;
};

struct typ {
  enum { TypeErr, TypeVoid, TypeInt, TypePtr, TypeFloat, TypeStruc } typ;
  size_t len;
  union {
    struct {
      bool is_signed;
    } i;
  } val;
};

struct sym {
  struct typ *typ;
  char const *name;
};

#endif
