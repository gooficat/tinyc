#include "tok.h"

char const *const KEYWORDS[] = {
	"int",	  "char",	"short",	"long",

	"float",  "double",

	"auto",	  "static", "extern",	"inline",

	"return", "break",	"continue", "goto",

	"sizeof",
};

char const *const PUNCTUATORS[] = {
	";", ":", ",",

	"(", ")", "[", "]", "{", "}",
};
