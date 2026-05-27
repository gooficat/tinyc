#ifndef __LEX__H__
#define __LEX__H__

#include <stdio.h>

typedef enum {
	TOK_NONE,

} TokenType;

typedef struct {
	struct {
		TokenType type;
	} token;
	FILE  *in_file;
	char  *line, *skr;
	size_t line_num, col_num;
} Lexer;

void Lexer_Init(Lexer *lexer, FILE *in_file);
void Lexer_Next(Lexer *lexer);
void Lexer_Close(Lexer *lexer);

#endif
