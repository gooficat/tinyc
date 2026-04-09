#include "lexer.h"
#include "tokns.h"
#include "vectr.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tok_strm__init(tok_strm_s *ts, const char *pat)
{
	FILE *f;
	long  len, rlen;
	f = fopen(pat, "rt");
	if (f == NULL)
	{
		fprintf(stderr, "Failed to open file `%s`\n", pat);
		exit(EXIT_FAILURE);
	}
	fseek(f, 0l, SEEK_END);
	len = ftell(f);
	fseek(f, 0l, SEEK_SET);
	ts->buf		  = malloc(len + 1);
	rlen		  = (long)fread(ts->buf, 1, len, f);
	ts->buf[rlen] = '\0';
	if (rlen != len)
	{
		ts->buf = realloc(ts->buf, rlen + 1);
	}
	ts->skr = ts->buf;

	vec_init(ts->str_pool);
	vec_init(ts->ident_pool);

	tok_strm__next(ts);
}

void tok_strm__next(tok_strm_s *ts)
{
	long long i, j;
	while (isspace(*ts->skr))
	{
		++ts->skr;
	}
	if (*ts->skr == '\0')
	{
		ts->tok.typ = TOK_EOF;
		return;
	}

	if (*ts->skr == '"')
	{
		char *eos, *str;
		ts->tok.typ = TOK_STRLIT;
		++ts->skr;
		do
		{
			eos = strchr(ts->skr, '"');
		} while (*(eos - 1) != '\\');
		i	= eos - ts->skr + 1;
		str = malloc(i);
		memcpy(str, ts->skr, i);
		vec_push(ts->str_pool, str);
		ts->skr += i;
		return;
	}

	for (i = 0; i < NUM_OPERATORS; ++i)
	{
		j = strlen(OPERATORS[i]);
		if (memcmp(ts->skr, OPERATORS[i], j))
			continue;
		if (isalnum(OPERATORS[i][0]) && (isalnum(ts->skr[j]) || ts->skr[j] == '_'))
			continue;
		ts->skr += j;
		ts->tok.typ = TOK_OPER;
		ts->tok.num = i;
		return;
	}
	for (i = 0; i < NUM_KEYWORDS; ++i)
	{
		j = strlen(KEYWORDS[i]);
		if (memcmp(ts->skr, KEYWORDS[i], j))
			continue;
		if (isalnum(ts->skr[j]))
			continue;
		ts->skr += j;
		ts->tok.typ = TOK_KEYW;
		ts->tok.num = i;
		return;
	}
	for (i = 0; i < NUM_PUNCTUATORS; ++i)
	{
		j = strlen(PUNCTUATORS[i]);
		if (memcmp(ts->skr, OPERATORS[i], j))
			continue;
		// if (isalnum(PUNCTUATORS[i][0]) && (isalnum(ts->skr[j]) || ts->skr[j] == '_'))
		// 	continue;
		ts->skr += j;
		ts->tok.typ = TOK_PUNC;
		ts->tok.num = i;
		return;
	}
	//
}

void tok_strm__destroy(tok_strm_s *ts)
{
	///
}
