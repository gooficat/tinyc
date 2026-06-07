#ifndef __TREE__H__
#define __TREE__H__

#include "dict.h"
#include "val.h"
#include <stddef.h>

struct scope {
  struct ast *nodes;
  size_t num_nodes;
  struct sym *syms;
  size_t num_syms;
  struct scope *parnt;
};

struct comlist {
  struct ast *nodes;
  size_t num_nodes;
};

extern struct ast {
  enum {
    AstNone,
    AstScope,
    AstBinOp,
    AstUnOp,
    AstOrder,
    AstFunc,
    AstCond,
    AstRef,
    AstCall,
    AstConst
  } typ;
  union {
    struct scope scope;
    struct {
      enum operator operator;
      struct ast *left, *right;
    } binop;
    struct {
      enum operator operator;
      struct ast *node;
    } unop;
    struct {
      enum {
        OrderNone,
        OrderBreak,
        OrderContinue,
        OrderReturn,
        OrderGoto
      } ordr;
      union {
        struct ast *node;
        struct sym *sym;
      } val;
    } order;
    struct {
      struct sym *sym;
      struct scope body;
    } func;
    struct {
      enum condtyp {
        CondNone,
        CondIf,
        CondWhile,
        CondDo,
        CondFor,
        CondSwitch
      } typ;
      struct ast *cond, *body, *els;
    } cond;
    struct sym *ref;
    struct {
      struct ast *of;
      struct comlist args;
    } call;
    struct cnst *cnst;
  } val;
} tree;

void tree_parse(void);
void tree_print(void);

#endif
