#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

char const * const TOKENS[] = {
	"int",
	"char",
	"float",
	"void",
	"return",
	"goto",
	"if",
	"(",
	")",
	"{",
	"}",
	"[",
	"]",
	";",
	":",
	",",
	".",
	"*",
};

struct {
	struct {
		enum {
			TK_INT, //0
			TK_CHAR, // 1
			TK_FLOAT, // 2
			TK_VOID, // 3
			TK_RETURN, // 4
			TK_GOTO, // 5
			TK_IF, // 6
			TK_PAREN_L,//7
			TK_PAREN_R, // 8
			TK_BRACE_L,//9
			TK_BRACE_R,//10
			TK_BRACK_L,//11
			TK_BRACK_R,//12
			TK_SEMI,//13
			TK_COLON, //14
			TK_COMMA, //15
			TK_PERIOD, //16
			TK_STAR, // 17

			TK_EOF,//18
			TK_CONSTANT,//19
			TK_IDENTIFIER,//20
			} type;
		size_t value;
	} token;
	char buffer[4096];
	size_t line, col;
} lexer = {0};

struct {
	struct {
		char **elems;
		size_t len;
	} identifiers;
	struct {
		struct c_const {
			enum {
				C_CONST_INT,
				C_CONST_FLOAT,
				C_CONST_STRING,
			} type;
			union {
				long long i;
				long double f;
				char const *s;
			} value;
		} *elems;
		size_t len;
	} constants;
} pool = {0};

struct frame {
	struct {
		struct c_var {
			char const *name;
			enum {
				C_VAR_TYPEDEF,
				C_VAR_STATIC,
				C_VAR_INLINE,
				C_VAR_AUTO,
			} storage;
			struct c_type {
				enum {
					C_TYPE_NONE,
					C_TYPE_ERR,
					C_TYPE_INT,
					C_TYPE_FLOAT,
					C_TYPE_STRUC,
					C_TYPE_ARRAY,
					C_TYPE_POINTER,
					C_TYPE_CHAR,
					C_TYPE_VOID,
				} type;
				int sign;
				enum {
					LENGTH_NONE,
					LENGTH_SHORT,
					LENGTH_LONG,
					LENGTH_LONG_LONG,
				} length;
				int is_const;
				struct c_type *next;
				size_t members;
				// TODO params
			} type;
		} *elems;
		size_t len;
	} vars;
	struct frame *previous;
} frame;

enum {
	ERR_INTERNAL,
	ERR_LINE_TOO_LONG,
	ERR_UNEXPECTED_TOKEN,
};

void error(int type) {
	switch (type) {
		case ERR_INTERNAL:
			fprintf(stderr, "INTERNAL ERROR\n");
			exit(EXIT_FAILURE);
		case ERR_LINE_TOO_LONG:
			fprintf(stderr, "ERROR: LINE %zu IS TOO LONG OR FILE DOES NOT END IN AN EMPTY LINE\n", lexer.line);
			exit(EXIT_FAILURE);
		case ERR_UNEXPECTED_TOKEN:
			fprintf(stderr, "UNEXPECTED TOKEN AT LINE %zu COL %zu\n", lexer.line, lexer.col);
			exit(EXIT_FAILURE);
	}
}

void lex_next(void) {
repeat:
	if (!lexer.buffer[lexer.col]) {
		if (!fgets(lexer.buffer, 4096, stdin)) {
			puts("eof");
			lexer.token.type = TK_EOF;
			return;
		}
		++lexer.line;
		puts("eol");
		if (lexer.buffer[strlen(lexer.buffer) - 1] != '\n') {
			error(ERR_LINE_TOO_LONG);
		}
		lexer.col = 0;
	}
	
	if (isspace(lexer.buffer[lexer.col])) {
		++lexer.col;
		goto repeat;
	}

	printf("token starting at %s", lexer.buffer + lexer.col);

	if (lexer.buffer[lexer.col] == '"') {
		// puts("str");
		// lex_next();

		// for (lexer.token.value = 0; lexer.token.value < pool.constants.len; ++lexer.token.value) {
		//     if (pool.constants.elems[lexer.token.value].type == C_CONST_STRING) {
		//         size_t len = strlen(TOKENS[lexer.token.type]);

		//         if (memcmp(lexer.buffer + lexer.col, TOKENS[lexer.token.type], len)) {
		//             continue;
		//         }

		//         if (isalnum(TOKENS[lexer.token.type][len - 1]) && isalnum(lexer.buffer[lexer.col + len])) {
		//             continue;
		//         }

		//         lexer.col += len;

		//         return;
		//     }
		// }

		// pool.constants.elems = realloc(pool.constants.elems, pool.constants.len * sizeof *pool.constants.elems);

		// pool.constants.elems[lexer.token.value].type = C_CONST_STRING;

		// return;
		fprintf(stderr, "NO STRINGS!!!");
		error(ERR_INTERNAL);
	}

	if (isdigit(lexer.buffer[lexer.col])) {
		puts("num");
		struct c_const constant;
		size_t ahead = lexer.col;
		while (isdigit(++ahead));
		lexer.token.type = TK_CONSTANT;

		if (lexer.buffer[ahead] == '.') {
			constant.type = C_CONST_FLOAT;
			constant.value.f = strtod(lexer.buffer, NULL);
			while (isalnum(lexer.buffer[++ahead]));
		}
		else {
			constant.type = C_CONST_INT;
			constant.value.i = strtol(lexer.buffer, NULL, 0);
		}
		
		lexer.col = ahead;

		for (lexer.token.value = 0; lexer.token.value < pool.constants.len; ++lexer.token.value) {
			if (pool.constants.elems[lexer.token.value].type == constant.type) {
				if (constant.type == C_CONST_FLOAT) {
					if (constant.value.f == pool.constants.elems[lexer.token.value].value.f) {
						return;
					}
				} else if (constant.type == C_CONST_INT) {
					if (constant.value.i == pool.constants.elems[lexer.token.value].value.i) {
						return;
					}
				}
			}
		}

		pool.constants.elems = realloc(pool.constants.elems, pool.constants.len * sizeof *pool.constants.elems);

		pool.constants.elems[lexer.token.value] = constant;

		return;
	}

	for (lexer.token.type = 0; lexer.token.type < sizeof(TOKENS) / sizeof(*TOKENS); ++lexer.token.type) {
		size_t len = strlen(TOKENS[lexer.token.type]);

		if (memcmp(lexer.buffer + lexer.col, TOKENS[lexer.token.type], len)) {
			continue;
		}

		if (isalnum(TOKENS[lexer.token.type][len - 1]) && isalnum(lexer.buffer[lexer.col + len])) {
			continue;
		}

		puts("token");
		lexer.col += len;

		return;
	}

	lexer.token.type = TK_IDENTIFIER;
	for (lexer.token.value = 0; lexer.token.value < pool.identifiers.len; ++lexer.token.value) {
		size_t len = strlen(pool.identifiers.elems[lexer.token.value]);

		if (memcmp(lexer.buffer + lexer.col, pool.identifiers.elems[lexer.token.value], len)) {
			break;
		}
		if (isalnum(pool.identifiers.elems[lexer.token.value][len - 1])) {
			break;
		}

		puts("iden");
		return;
	}
	
	puts("new iden");
	pool.identifiers.elems = realloc(pool.identifiers.elems, ++pool.identifiers.len * sizeof *pool.identifiers.elems);
	
	size_t iden_len = 0;
	while (isalnum(lexer.buffer[lexer.col + ++iden_len]));

	pool.identifiers.elems[lexer.token.value] = malloc(iden_len + 1);

	memcpy(pool.identifiers.elems[lexer.token.value], lexer.buffer + lexer.col, iden_len);
	pool.identifiers.elems[lexer.token.value][iden_len] = '\0';

	lexer.col += iden_len;
}

void gen_decl(struct c_var* var) {
	while (lexer.token.type <= TK_VOID && lexer.token.type >= TK_INT) {
		switch (lexer.token.type) {
			case TK_INT:
				var->type.type = C_TYPE_INT;
				break;
			case TK_CHAR:
				var->type.type = C_TYPE_CHAR;
				break;
			case TK_FLOAT:
				var->type.type = C_TYPE_FLOAT;
				break;
			case TK_VOID:
				var->type.type = C_TYPE_VOID;
				break;
		}
		if (lexer.token.type == TK_STAR) {
			var->type.next = calloc(1, sizeof *var->type.next);
			*var->type.next = var->type;
			var->type.type = C_TYPE_POINTER;
		}
	}
	var->name = pool.identifiers.elems[lexer.token.value];
	lex_next();

	if (lexer.token.type == TK_PAREN_L) {
		{
			struct frame back = frame;
			memset(&frame, 0, sizeof frame);
			frame.previous = malloc(sizeof *frame.previous);
			*frame.previous = back;
		}

		for (;;) {
			size_t idx = frame.vars.len++;
			frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);
			gen_decl(frame.vars.elems + idx);
			// OH MY GOD
			// I GET IT
			if (lexer.token.type == TK_COMMA) {
				lex_next();
			} else if (lexer.token.type == TK_PAREN_R) {
				break;
			} else {
				error(ERR_UNEXPECTED_TOKEN);
			}
		}
		
		if (lexer.token.type == TK_BRACE_L) {
			if (frame.previous != NULL) {
				error(ERR_UNEXPECTED_TOKEN);
			}

			lex_next();

			while (lexer.token.type != TK_BRACE_R) {
			size_t idx = frame.vars.len++;
			frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);
			
			gen_decl(frame.vars.elems + idx);
			}
		}
	}
}

int main(void) {
	frame.previous = NULL;

	puts(
		".code64\n"
		".section \".text\"");
	
	lex_next();
	while (lexer.token.type != TK_EOF) {
		if (lexer.token.type <= TK_VOID && lexer.token.type >= TK_INT) {
			size_t idx = frame.vars.len++;
			frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);
			
			gen_decl(frame.vars.elems + idx);

		} else if (lexer.token.type == TK_SEMI) {
			lex_next();
		} else {
			error(ERR_UNEXPECTED_TOKEN);
		}
	}

	puts(".section \".data\"");
	for (size_t i = 0; i < pool.constants.len; ++i) {
		printf("C@CONST_%zu:\n\t", i);
		switch (pool.constants.elems[i].type) {
			case C_CONST_INT:
				printf(".quad %lld", pool.constants.elems[i].value.i);
				break;
			case C_CONST_FLOAT:
				printf(".double %LF", pool.constants.elems[i].value.f);
				break;
			case C_CONST_STRING:
				printf(".asciz %s", pool.constants.elems[i].value.s);
				break;
		}
		putchar('\n');
	}
}