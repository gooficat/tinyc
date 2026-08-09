#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

char const *const TOKENS[] = {
	"int",
	"char",
	"float",
	"void",
	"static",
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
	"=",
	"*",
	"&",
	"-",
	"+",
	"!",
};

struct
{
	struct
	{
		enum
		{
			TK_INT,
			TK_CHAR,
			TK_FLOAT,
			TK_VOID,
			TK_STATIC,
			TK_RETURN,
			TK_GOTO,
			TK_IF,
			TK_PAREN_L,
			TK_PAREN_R,
			TK_BRACE_L,
			TK_BRACE_R,
			TK_BRACK_L,
			TK_BRACK_R,
			TK_SEMI,
			TK_COLON,
			TK_COMMA,
			TK_PERIOD,
			TK_EQU,
			TK_ASTER,
			TK_AMPER,
			TK_MINUS,
			TK_PLUS,
			TK_LOG_NOT,
			TK_EOF,
			TK_CONSTANT,
			TK_IDENTIFIER,
		} type;
		size_t value;
	} token;
	char buffer[4096];
	size_t line, col;
} lexer = {0};

struct
{
	struct
	{
		char **elems;
		size_t len;
	} identifiers;
	struct
	{
		struct c_const
		{
			enum
			{
				C_CONST_INT,
				C_CONST_FLOAT,
				C_CONST_STRING,
			} type;
			union
			{
				long long i;
				long double f;
				char const *s;
			} value;
		} *elems;
		size_t len;
	} constants;
} pool = {0};

struct frame
{
	struct
	{
		struct c_var
		{
			char const *name;
			enum
			{
				C_VAR_TYPEDEF,
				C_VAR_STATIC,
				C_VAR_INLINE,
				C_VAR_AUTO,
				C_VAR_REGISTER,
			} storage;
			struct c_type
			{
				enum
				{
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
				enum
				{
					LENGTH_NONE,
					LENGTH_SHORT,
					LENGTH_LONG,
					LENGTH_LONG_LONG,
				} length;
				int is_const;
				struct c_type *next;
				size_t members;
				size_t size_of;
				// TODO params
			} type;
			size_t offset;
		} *elems;
		size_t len;
	} vars;
	size_t total_offset;
	struct frame *previous;
} frame;

enum
{
	ERR_INTERNAL,
	ERR_LINE_TOO_LONG,
	ERR_UNEXPECTED_TOKEN,
};

size_t disamBIG(void)
{
	static size_t ctr = 0;
	return ctr++;
}

void error(int type)
{
	switch (type)
	{
	case ERR_INTERNAL:
		fprintf(stderr, ";INTERNAL ERROR\n");
		exit(EXIT_FAILURE);
	case ERR_LINE_TOO_LONG:
		fprintf(stderr, ";ERROR: LINE %zu IS TOO LONG OR FILE DOES NOT END IN AN EMPTY LINE\n", lexer.line);
		exit(EXIT_FAILURE);
	case ERR_UNEXPECTED_TOKEN:
		fprintf(stderr, ";UNEXPECTED TOKEN OF TYPE %d AT LINE %zu COL %zu\n", lexer.token.type, lexer.line, lexer.col);
		exit(EXIT_FAILURE);
	}
}

void lex_next(void)
{
repeat:
	if (!lexer.buffer[lexer.col])
	{
		if (!fgets(lexer.buffer, 4096, stdin))
		{
			//			puts(";eof");
			lexer.token.type = TK_EOF;
			return;
		}
		++lexer.line;
		//		puts(";eol");
		if (lexer.buffer[strlen(lexer.buffer) - 1] != '\n')
		{
			error(ERR_LINE_TOO_LONG);
		}
		lexer.col = 0;
	}

	if (isspace(lexer.buffer[lexer.col]))
	{
		++lexer.col;
		goto repeat;
	}

	//	printf(";token starting at %s", lexer.buffer + lexer.col);

	if (lexer.buffer[lexer.col] == '"')
	{
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
		//		fprintf(stderr, ";NO STRINGS!!!");
		error(ERR_INTERNAL);
	}

	if (isdigit(lexer.buffer[lexer.col]))
	{
		//		puts(";num");
		struct c_const constant;
		size_t ahead = lexer.col;
		while (isdigit(++ahead))
			;
		lexer.token.type = TK_CONSTANT;

		if (lexer.buffer[ahead] == '.')
		{
			constant.type = C_CONST_FLOAT;
			constant.value.f = strtod(lexer.buffer, NULL);
			while (isalnum(lexer.buffer[++ahead]))
				;
		}
		else
		{
			constant.type = C_CONST_INT;
			constant.value.i = strtol(lexer.buffer, NULL, 0);
		}

		lexer.col = ahead;

		for (lexer.token.value = 0; lexer.token.value < pool.constants.len; ++lexer.token.value)
		{
			if (pool.constants.elems[lexer.token.value].type == constant.type)
			{
				if (constant.type == C_CONST_FLOAT)
				{
					if (constant.value.f == pool.constants.elems[lexer.token.value].value.f)
					{
						return;
					}
				}
				else if (constant.type == C_CONST_INT)
				{
					if (constant.value.i == pool.constants.elems[lexer.token.value].value.i)
					{
						return;
					}
				}
			}
		}

		pool.constants.elems = realloc(pool.constants.elems, ++pool.constants.len * sizeof *pool.constants.elems);

		pool.constants.elems[lexer.token.value] = constant;

		return;
	}

	for (lexer.token.type = 0; lexer.token.type < sizeof(TOKENS) / sizeof(*TOKENS); ++lexer.token.type)
	{
		size_t len = strlen(TOKENS[lexer.token.type]);

		if (memcmp(lexer.buffer + lexer.col, TOKENS[lexer.token.type], len))
		{
			continue;
		}

		if (isalnum(TOKENS[lexer.token.type][len - 1]) && isalnum(lexer.buffer[lexer.col + len]))
		{
			continue;
		}

		//		puts(";token");
		lexer.col += len;

		return;
	}

	lexer.token.type = TK_IDENTIFIER;
	for (lexer.token.value = 0; lexer.token.value < pool.identifiers.len; ++lexer.token.value)
	{
		size_t len = strlen(pool.identifiers.elems[lexer.token.value]);

		if (memcmp(lexer.buffer + lexer.col, pool.identifiers.elems[lexer.token.value], len))
		{
			break;
		}
		if (isalnum(pool.identifiers.elems[lexer.token.value][len - 1]))
		{
			break;
		}

		//		puts(";iden");
		return;
	}

	//	puts(";new iden");
	pool.identifiers.elems = realloc(pool.identifiers.elems, ++pool.identifiers.len * sizeof *pool.identifiers.elems);

	size_t iden_len = 0;
	while (isalnum(lexer.buffer[lexer.col + ++iden_len]))
		;

	pool.identifiers.elems[lexer.token.value] = malloc(iden_len + 1);

	memcpy(pool.identifiers.elems[lexer.token.value], lexer.buffer + lexer.col, iden_len);
	pool.identifiers.elems[lexer.token.value][iden_len] = '\0';

	lexer.col += iden_len;
}

struct c_var *find_var_rec(char *name, struct frame *frame)
{
	for (size_t i = 0; i < frame->vars.len; ++i)
	{
		if (!strcmp(name, frame->vars.elems[i].name))
		{
			return frame->vars.elems + i;
		}
	}
	if (frame->previous)
	{
		return find_var_rec(name, frame->previous);
	}
	return NULL;
}

struct c_var *find_var(char *name)
{
	return find_var_rec(name, &frame);
}

void gen_expr(void)
{
	switch ((int)lexer.token.type)
	{
	case TK_RETURN:
		lex_next();
		gen_expr();
		printf("\tpop %%rbp\n"
			   "\tret\n");
		break;
	case TK_GOTO:
		// TODO frames
		break;
	case TK_BRACE_L:
		break;
	case TK_BRACK_L:
		break;
	case TK_PAREN_L:
		lex_next();
		gen_expr();
		// if () // tODO
		lex_next();
		break;
	case TK_IF:
		break;
	case TK_ASTER:
		lex_next();
		gen_expr();
		if (lexer.token.type == TK_EQU)
		{
			puts("\tpush %rax");
			lex_next();
			gen_expr();
			puts("\tpop %rbx\n"
				 "\tmov (%rbx), %rax");
		}
		else
		{ // TODO
			puts("\tmov %rax, (%rax)");
		}
		break;
	case TK_AMPER:
		lex_next();

	case TK_IDENTIFIER:
	{ // LEFT OFF HERE TODO TODO TODO
		struct c_var *var = find_var(pool.identifiers.elems[lexer.token.value]);
		lex_next();
		if (var->storage == C_VAR_TYPEDEF)
		{
			error(ERR_INTERNAL);
		}
		else if (lexer.token.type == TK_EQU)
		{
			lex_next();
			gen_expr();
			printf("\tmov ");
			switch ((int)var->storage)
			{
			case C_VAR_AUTO:
				printf("-%zu(%%rbp)", var->offset);
				break;
			case C_VAR_STATIC:
				printf("\"%s\"", var->name);
				break;
				// case C_VAR_REGISTER:
			}
			// TODO big stuff
			printf(", %%rax\n");
		}
		else if (lexer.token.type == TK_PAREN_L)
		{
			lex_next();
			// TODO calls
			error(ERR_INTERNAL);
		}
		else
		{
			printf("\tmov %%rax, ");
			switch ((int)var->storage)
			{
			case C_VAR_AUTO:
				printf("-%zu(%%rbp)", var->offset);
				break;
			case C_VAR_STATIC:
				printf("\"C@STAT_%zu\"(%%rip)", var->offset);
				break;
				// case C_VAR_REGISTER:
			}
			putchar('\n');
		}
	}
	break;
	case TK_CONSTANT:
		printf("\tmov %%rax, \"C@CONST_%zu\"(%%rip)\n", lexer.token.value);
		lex_next();
		break;
	}
}

void gen_decl(struct c_var *var)
{
	//	printf(";new decl\n");
	while (lexer.token.type <= TK_VOID && lexer.token.type >= TK_INT)
	{
		switch ((int)lexer.token.type)
		{
		case TK_INT:
			var->type.type = C_TYPE_INT;
			var->type.size_of = 8;
			break;
		case TK_CHAR:
			var->type.type = C_TYPE_CHAR;
			var->type.size_of = 1;
			break;
		case TK_FLOAT:
			var->type.type = C_TYPE_FLOAT;
			var->type.size_of = 8;
			break;
		case TK_VOID:
			var->type.type = C_TYPE_VOID;
			var->type.size_of = 0;
			break;
		}
		lex_next();
		if (lexer.token.type == TK_ASTER)
		{
			var->type.next = calloc(1, sizeof *var->type.next);
			*var->type.next = var->type;
			var->type.type = C_TYPE_POINTER;
		}
	}
	var->name = pool.identifiers.elems[lexer.token.value];
	//	printf(";new decl is named %s\n", var->name);
	lex_next();

	if (lexer.token.type == TK_PAREN_L)
	{
		var->storage = C_VAR_STATIC;
		struct
		{
			struct c_var *elems;
			size_t len;
		} params = {0};
		//		printf(";new decl is a function\n");
		lex_next();
		if (lexer.token.type != TK_PAREN_R)
		{
			//	printf(";new decl is a function with params\n");
			for (;;)
			{
				size_t idx = params.len++;
				params.elems = realloc(params.elems, params.len * sizeof *params.elems);
				gen_decl(params.elems + idx);

				if (params.elems[idx].storage == C_VAR_AUTO)
				{
					params.elems[idx].offset = frame.total_offset;
					frame.total_offset += params.elems[idx].type.size_of;
				}

				if (lexer.token.type == TK_COMMA)
				{
					lex_next();
				}
				else if (lexer.token.type == TK_PAREN_R)
				{
					break;
				}
				else
				{
					error(ERR_UNEXPECTED_TOKEN);
				}
			}
		}
		lex_next();
		if (lexer.token.type == TK_BRACE_L)
		{
			if (frame.previous != NULL)
			{
				error(ERR_UNEXPECTED_TOKEN);
			}
			printf(".globl \"%s\"\n", var->name);
			printf("\"%s\":\n", var->name);
			{
				struct frame back = frame;
				memset(&frame, 0, sizeof frame);
				frame.previous = malloc(sizeof *frame.previous);
				*frame.previous = back;
			}

			for (size_t i = 0; i < params.len; ++i)
			{
				size_t idx = frame.vars.len++;
				frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);
				frame.vars.elems[idx] = params.elems[i];
			}

			lex_next();

			{
				size_t stackoff = 0;
				while (lexer.token.type <= TK_VOID && lexer.token.type >= TK_INT)
				{
					size_t idx = frame.vars.len++;
					frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);
					gen_decl(frame.vars.elems + idx);

					if (frame.vars.elems[idx].storage == C_VAR_AUTO)
					{
						frame.vars.elems[idx].offset = frame.total_offset;
						frame.total_offset += frame.vars.elems[idx].type.size_of; // TODO fix this gibberish
						stackoff += frame.vars.elems[idx].type.size_of;
					}
				}
				printf("\tsub %%rsp, %zu\n", stackoff);
			}

			puts("\tpush %rbp\n"
				 "\tmov %rbp, %rsp");

			while (lexer.token.type != TK_BRACE_R)
			{
				if (lexer.token.type == TK_SEMI)
				{
					lex_next();
				}
				else
				{
					gen_expr();
				}
			}
			lex_next();
			puts("\tpop %rbp\n"
				 "\tret");
			{
				for (size_t i = 0; i < frame.vars.len; ++i)
				{
					if (frame.vars.elems[i].storage == C_VAR_STATIC)
					{
						printf("\"C@STAT_%zu\":\n"
							   "\t.space %zu\n",
							   frame.vars.elems[i].offset,
							   frame.vars.elems[i].type.size_of);
					}
				}

				struct frame previous = *frame.previous;
				free(frame.previous);
				free(frame.vars.elems);
				frame = previous;
			}
		}
	}
	else
	{
		if (frame.previous != NULL)
		{
			var->storage = C_VAR_AUTO;
		}
		else
		{
			var->storage = C_VAR_STATIC; // WRONG! SHOULD BE EXTERN AND HAVE A CHECK IF ITS ARLEADY SET
			printf(".globl \"%s\"\n", var->name);
			printf("\"%s\":\n", var->name);
			printf("\t.space %zu\n", var->type.size_of);
		}
	}
	if (lexer.token.type == TK_SEMI)
	{
		lex_next();
	}
	else if (lexer.token.type == TK_COMMA)
	{
		// TODO
		error(ERR_INTERNAL);
	}
}

int main(void)
{
	frame.previous = NULL;

	puts(
		".code64\n"
		".section \".text\"");

	lex_next();
	while (lexer.token.type != TK_EOF)
	{
		if (lexer.token.type <= TK_VOID && lexer.token.type >= TK_INT)
		{
			size_t idx = frame.vars.len++;
			frame.vars.elems = realloc(frame.vars.elems, frame.vars.len * sizeof *frame.vars.elems);

			gen_decl(frame.vars.elems + idx);
		}
		else if (lexer.token.type == TK_SEMI)
		{
			lex_next();
		}
		else
		{
			error(ERR_UNEXPECTED_TOKEN);
		}
	}

	puts(".section \".rodata\"");
	for (size_t i = 0; i < pool.constants.len; ++i)
	{
		printf("\"C@CONST_%zu\":\n\t", i);
		switch (pool.constants.elems[i].type)
		{
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
