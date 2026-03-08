#pragma once

#include "tok.h"
typedef struct
{
	tokstrm ts;
} lexer;

void lexerinit(lexer *l, const char *fpath);