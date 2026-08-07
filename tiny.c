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

            TK_EOF,//14
            TK_CONSTANT,//15
            TK_IDENTIFIER,//16
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
                int i;
                float f;
                char const *s;
            } val;
        } *elems;
        size_t len;
    } constants;
} pool = {0};


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

    printf("Tok starting at %s", lexer.buffer + lexer.col);

    if (isdigit(lexer.buffer[lexer.col])) {
        puts("num");
        struct c_const constant;
        size_t ahead = lexer.col;
        while (isdigit(++ahead));
        lexer.token.type = TK_CONSTANT;

        if (lexer.buffer[ahead] == '.') {
            constant.type = C_CONST_FLOAT;
            constant.val.f = strtod(lexer.buffer, NULL);
            while (isalnum(lexer.buffer[++ahead]));
        }
        else {
            constant.type = C_CONST_INT;
            constant.val.i = strtol(lexer.buffer, NULL, 0);
        }
        
        lexer.col = ahead;

        for (lexer.token.value = 0; lexer.token.value < pool.constants.len; ++lexer.token.value) {
            if (pool.constants.elems[lexer.token.value].type == constant.type) {
                if (constant.type == C_CONST_FLOAT) {
                    if (constant.val.f == pool.constants.elems[lexer.token.value].val.f) {
                        return;
                    }
                } else if (constant.type == C_CONST_INT) {
                    if (constant.val.i == pool.constants.elems[lexer.token.value].val.i) {
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

        puts("tok");
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

int main(void) {
    lex_next();
    do {
        printf("%d\n", lexer.token.type);
        lex_next();
    } while (lexer.token.type != TK_EOF);
}