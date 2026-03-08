#pragma once

#include "tok.h"
#include <stddef.h>
#include <stdio.h>

typedef struct fbufstate
{
	FILE *fptr;
	int ch;
	size_t lnum;
	struct fbufstate *back;
} fbufstate;

typedef struct
{
	fbufstate *fbs;
	char *tbuf;
} tokstrm;

void fbufstateinit(fbufstate *fbs, const char *fpath);
void fbufstatetrim(fbufstate *fbs);
void fbufgetc(fbufstate *fbs);

void tokstrminit(tokstrm *ts, const char *fpath);
void tokstrmnext(tokstrm *ts);
void tokstrmpush(tokstrm *ts, const char *fpath);
void tokstrmpop(tokstrm *ts);
