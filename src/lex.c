#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

char buff[LINE_MAX] = {[0] = 0};

size_t line = 0, col = 0;

struct tok tok;

void lex_next(void)
{
    size_t len;
repeat:
    if (buff[col] == 0)
    {
        if (!fgets(buff, LINE_MAX, stdin))
        {
            tok.type = TOK_EOF;
            return;
        }
        else if (buff[LINE_MAX - 1] != 0)
        {
        }
        else
        {
            ++line;
            col = 0;
        }
    }
    if (isspace(buff[col]))
    {
        ++col;
        goto repeat;
    }

    if (isdigit(buff[col]))
    {
        struct cval val;
        tok.type = TOK_VAL;
        len = col;
        while (isalnum(buff[col]))
        {
            ++col;
        }
        if (buff[col] == '.')
        {
            val.type = CVAL_FLT;
            val.val.f = strtod(buff + len, NULL);
            do
            {
                ++col;
            } while (isdigit(buff[col]));
        }
        else
        {
            val.type = CVAL_INT;
            val.val.i = strtol(buff + len, NULL, 0);
        }

        for (tok.val = 0; tok.val < cvals.len; ++tok.val)
        {
            if (cvals.els[tok.val].type == val.type)
            {
                switch (cvals.els[tok.val].type)
                {
                case CVAL_INT:
                    if (cvals.els[tok.val].val.i == val.val.i)
                    {
                        return;
                    }
                    else
                    {
                        break;
                    }
                case CVAL_FLT:
                    if (cvals.els[tok.val].val.f == val.val.f)
                    {
                        return;
                    }
                    else
                    {
                        break;
                    }
                default:
                    break;
                }
            }
        }

        cvals.els = realloc(cvals.els, ++cvals.len);
        cvals.els[tok.val] = val;
        return;
    }

    if (buff[col] == '"')
    {
        tok.type = TOK_VAL;
        return;
    }

    for (tok.type = 0; TOKS[tok.type]; ++tok.type)
    {
        len = strlen(TOKS[tok.type]);
        if (!memcmp(TOKS[tok.type], buff + col, len) &&
            ((buff[col + len - 1] != '_' && !isalnum(buff[col + len - 1])) ||
             (buff[col + len] != '_' && !isalnum(buff[col + len]))))
        {
            col += len;
            return;
        }
    }

    tok.type = TOK_IDEN;

    for (tok.val = 0; tok.val < csyms.len; ++tok.val)
    {
        len = strlen(csyms.els[tok.val]);
        if (!memcmp(csyms.els[tok.val], buff + col, len) &&
            buff[col + len] != '_' && !isalnum(buff[col + len]))
        {
            col += len;
            return;
        }
    }
    csyms.els = realloc(csyms.els, ++csyms.len * sizeof *csyms.els);

    len = 0;
    while (buff[col + len] == '_' || isalnum(buff[col + len]))
    {
        ++len;
    }
    csyms.els[tok.val] = malloc(len + 1);
    memcpy(csyms.els[tok.val], buff + col, len);
    csyms.els[tok.val][len] = 0;
    col += len;
}
