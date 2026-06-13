#include "tok.h"
#include <search.h>

char const *const KEYWORDS[] = {
	"int",	  "char",	"short",	"long",

	"float",  "double",

	"auto",	  "static", "extern",	"inline",

	"return", "break",	"continue", "goto",

	"sizeof", NULL,
};

char const *const PUNCTUATORS[] = {
	";", ":", ",",

	"(", ")", "[", "]", "{", "}", NULL,
};
