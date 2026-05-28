#ifndef __LEX__H__
#define __LEX__H__

#include <stdio.h>

typedef enum {
	TOK_NONE,
	TOK_PUNC
} TokenType;

typedef struct {
	struct {
		TokenType type;
	} token;
	FILE *in_file;
	char *skr, *line;
} Lexer;

void Lexer_Init(Lexer *lexer, FILE *in_file);
void Lexer_Next(Lexer *lexer);

#endif
