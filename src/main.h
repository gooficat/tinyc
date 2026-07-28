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

extern struct csym_list
{
    char **els;
    size_t len;
} csyms;

void lex_next(void);

extern char const *const TOKS[];

#endif //!__MAIN__H__
