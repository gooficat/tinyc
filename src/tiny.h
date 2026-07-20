#ifndef __TINY__H__
#define __TINY__H__

#include <stddef.h>
#include <stdio.h>

#define line_max_len 4096

struct token {
    enum {

#define tk(id) tok_##id,
#include "toks.h"
#undef tk

        tok_eof,
        tok_value,
        tok_ident,
    } type;
    size_t value;
};

struct file_info {
    FILE* fptr;
    size_t line;
    size_t col;
};

struct lexer {
    struct token token;
    struct file_info file;
    char line[line_max_len];
};


struct stack_frame {
    //
};

struct codegen {
    struct file_info file;

};

#endif  //!__TINY__H__
