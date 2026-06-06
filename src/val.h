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
  enum {
    TypeNone,
    TypeErr,
    TypeVoid,
    TypeInt,
    TypePtr,
    TypeFloat,
    TypeStruc,
    TypeLabel
  } typ;
  size_t len;
  union {
    struct {
      unsigned char signedness;
    } i;
    struct typ *p;
  } val;
  struct sym *args;
  size_t num_args;
};

struct sym {
  struct typ *typ;
  char const *name;
};

#endif
