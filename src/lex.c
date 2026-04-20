#include "lex.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

FILE  *f;
char   line[CL_MAX_LINE];
size_t line_num;
char  *skr;

void cl_getline(void)
{
	skr = fgets(line, CL_MAX_LINE, f);
	if (line[CL_MAX_LINE - 1] == '\0')
	{
		fprintf(stderr, "Excessively long line at %zu\n", line_num);
		exit(EXIT_FAILURE);
	}
	if (skr == NULL)
	{
		fprintf(stderr, "File must end in a new line\n");
		exit(EXIT_FAILURE);
	}
	++line_num;
}

void cl_open(char const *path)
{
	f = fopen(path, "rt");
	if (!f)
	{
		fprintf(stderr, "Failed to open file %s for reading\n", path);
		exit(EXIT_FAILURE);
	}
	line_num = 0;
}

bool tok_cmp(char *t)
{
	char *s = skr;
	while (*s++ == *t++)
		;
	if (*s)
	{
		return false;
	}
}

void cl_next(void)
{
}
