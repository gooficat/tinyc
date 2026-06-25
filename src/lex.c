#include "lex.h"
#include "tiny.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *file;

char line[LINE_BUF_LEN];
size_t line_num;
size_t col_num;
tok_s tok;
