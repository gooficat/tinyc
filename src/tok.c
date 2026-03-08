#include "tok.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void fbufstateinit(fbufstate *fbs, const char *fpath)
{
	fopen_s(&fbs->fptr, fpath, "rt");
	fbufgetc(fbs);
	fbs->lnum = 1;
	fbs->back = NULL;
}

void fbufstatetrim(fbufstate *fbs)
{
	while (isspace(fbs->ch))
	{
		fbufgetc(fbs);
	}
}

void fbufgetc(fbufstate *fbs)
{
	fbs->ch = fgetc(fbs->fptr);
	if (fbs->ch == '\n')
	{
		fbs->lnum++;
	}
}

void tokstrminit(tokstrm *ts, const char *fpath)
{
	ts->fbs = malloc(sizeof(*ts->fbs));
	fbufstateinit(ts->fbs, fpath);
	tokstrmnext(ts);
}

void tokstrmpop(tokstrm *ts)
{
	fbufstate *fbs = ts->fbs->back;
	free(ts->fbs);
	ts->fbs = fbs;
}

void tokstrmnext(tokstrm *ts)
{
postpop:
	fbufstatetrim(ts->fbs);

	char *t = &ts->tbuf[0];
	if (ts->fbs->ch == EOF)
	{
		if (ts->fbs->back)
		{
			tokstrmpop(ts);
			goto postpop;
		}
	}
	else if (ts->fbs->ch == '"')
	{
		do
		{
			*t++ = ts->fbs->ch;
			fbufgetc(ts->fbs);
		} while (ts->fbs->ch != '"');
		fbufgetc(ts->fbs);
	}
	else if (!isalnum(ts->fbs->ch))
	{
		*t++ = ts->fbs->ch;
		fbufgetc(ts->fbs);
	}
	else
	{
		do
		{
			*t++ = ts->fbs->ch;
			fbufgetc(ts->fbs);
		} while (isalnum(ts->fbs->ch));
	}
	*t = 0;
}
void tokstrmpush(tokstrm *ts, const char *fpath);