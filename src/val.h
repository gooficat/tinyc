#ifndef __VAL__H__
#define __VAL__H__
#include <stdint.h>
#include <vcruntime.h>

typedef uint8_t bool;

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
      bool is_signed;
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
      size_t num_members;
    } s;
  } val;
  bool is_function;
  bool is_const;
  struct CType *args;
  size_t num_args;
} CType;

typedef struct {
  char const *name;
  CType *type;
} CVariable;

typedef enum {
  CCONST_NONE,
  CCONST_INTEG,
  CCONST_FLOAT,
  CCONST_STRING,
  CCONST_COMPOUND
} CConstType;

typedef struct CConst {
  CConstType type;
  union {
    int64_t i;
    long double f;
    char const *s;
    struct CConst *c;
  } val;
} CConst;

#endif
