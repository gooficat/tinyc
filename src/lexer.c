#include "lexer.h"
#include "tokns.h"
#include "vectr.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tok_strm__init(tok_strm *ts, const char *pat)
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
	ts->buf = malloc(len + 1);
	rlen	= (long)fread(ts->buf, 1, len, f);
	fclose(f);
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

int tok_strm__list_search(tok_strm *ts, const char **list, long long list_len)
{
	long long i, j;
	for (i = 0; i < list_len; ++i)
	{
		j = strlen(list[i]);
		if (memcmp(ts->skr, list[i], j))
			continue;
		if (isalnum(list[i][0]) && (isalnum(ts->skr[j]) || ts->skr[j] == '_'))
			continue;
		ts->skr += j;
		ts->tok.num = i;
		return 0;
	}
	return -1;
}

void tok_strm__next(tok_strm *ts)
{
	long long i, j;
	char	 *eos;

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
		char *str;
		ts->tok.typ = TOK_STRLIT;
		++ts->skr;
		do
		{
			eos = strchr(ts->skr, '"');
		} while (*(eos - 1) != '\\');
		i	= eos - ts->skr + 1;
		str = malloc(i + 1);
		memcpy(str, ts->skr, i);
		str[i] = '\0';
		ts->skr += i;
		for (i = 0; i < (long long)ts->str_pool.len; ++i)
		{
			j = strlen(ts->str_pool.data[i]);
			if (memcmp(str, ts->str_pool.data[i], j))
				continue;
			ts->tok.num = i;
			free(str);
			return;
		}
		ts->tok.num = ts->str_pool.len;
		vec_push(ts->str_pool, str);
		ts->tok.str = str;
		return;
	}

	if (isdigit(*ts->skr))
	{
		ts->tok.typ = TOK_CONST;
		ts->tok.num = strtoll(ts->skr, &ts->skr, 0);
		return;
	}

	if (tok_strm__list_search(ts, &OPERATORS[0], NUM_OPERATORS) != -1)
	{
		ts->tok.typ = TOK_OPER;
		return;
	}
	if (tok_strm__list_search(ts, &KEYWORDS[0], NUM_KEYWORDS) != -1)
	{
		ts->tok.typ = TOK_KEYW;
		return;
	}
	if (tok_strm__list_search(ts, &PUNCTUATORS[0], NUM_PUNCTUATORS) != -1)
	{
		ts->tok.typ = TOK_PUNC;
		return;
	}

	ts->tok.typ = TOK_IDENT;
	if (tok_strm__list_search(ts, (const char **)&ts->ident_pool.data[0], ts->ident_pool.len) != -1)
	{
		return;
	}
	eos = ts->skr;
	while (*eos == '_' || isalnum(*eos))
		++eos;
	i = eos - ts->skr;

	eos = malloc(i + 1);
	memcpy(eos, ts->skr, i);
	eos[i] = '\0';
	ts->skr += i;
	ts->tok.num = ts->ident_pool.len;
	vec_push(ts->ident_pool, eos);
}

void tok_strm__destroy(tok_strm *ts)
{
	unsigned long long i;
	free(ts->buf);
	for (i = 0; i < ts->str_pool.len; ++i)
	{
		free(ts->str_pool.data[i]);
	}
	for (i = 0; i < ts->ident_pool.len; ++i)
	{
		free(ts->ident_pool.data[i]);
	}
	free(ts->str_pool.data);
	free(ts->ident_pool.data);
}
