#ifndef __MAIN__H__
#define __MAIN__H__

#include <stddef.h>
#include <stdio.h>

#define LINE_MAX 4096

extern struct tok
{
    enum
    {
#define T(id, str) TOK_##id,
#include "tok.h"
#undef T
        TOK_EOF,
        TOK_VAL,
        TOK_IDEN,
    } type;
    size_t val;
} tok;

struct cval
{
    enum
    {
        CVAL_INT,
        CVAL_FLT,
        CVAL_ARR,
    } type;
    union
    {
        int i;
        float f;
        struct
        {
            struct cval *val;
            size_t len;
        } a;
    } val;
};

extern struct cval_list
{

    struct cval *els;
    size_t len;
} cvals;

extern struct sym_list
{
    char **els;
    size_t len;
} syms;

struct ctype
{
    enum
    {
        CTYPE_ERR,
        CTYPE_COM,
        CTYPE_NON,
        CTYPE_INT,
        CTYPE_FLT,
        CTYPE_PTR,
    } type;
    union
    {
        struct
        {
            struct ctype *under;
            size_t num;
        } com;
        struct ctype *ptr;
    } val;
};

enum cmem
{
    MEM_STK,
    MEM_STT,
    MEM_REG,
};

struct mem_desc
{
    enum cmem type;
    unsigned long long val;
};

struct csym_table
{
    struct
    {
        struct csym
        {
            char *name;
            enum cmem mem;
            struct ctype *type;
        } *els;
        size_t len;
    } syms;
    struct csym_table *prev;
};

void lex_next(void);

void find_sym(char *name, struct csym_table *tbl, struct mem_desc *dsc);
size_t intrin_sizeof(struct ctype *type);

extern char const *const TOKS[];

#endif //!__MAIN__H__
