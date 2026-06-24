#ifndef TK
#define TK(id, str)
#endif

TK(OP_DOT, ".")
TK(OP_ARROW, "->")
TK(OP_ARRAY, "[]")

TK(OP_ADD, "+")
TK(OP_SUB, "-")
TK(OP_MUL, "*")
TK(OP_DIV, "/")
TK(OP_MOD, "%")

TK(OP_BIN_AND, "&")
TK(OP_BIN_OR, "|")
TK(OP_BIN_XOR, "^")
TK(OP_BIN_NOT, "~")
TK(OP_BIN_SHL, "<<")
TK(OP_BIN_SHR, ">>")

TK(OP_LOG_AND, "&&")
TK(OP_LOG_OR, "||")
TK(OP_LOG_NOT, "!")
TK(OP_LOG_LEQ, "<=")
TK(OP_LOG_GEQ, ">=")
TK(OP_LOG_EQU, "==")
TK(OP_LOG_NEQ, "!=")

TK(OP_ASS, "=")

TK(PAREN_L, "(")
TK(PAREN_R, ")")
TK(BRACK_L, "{")
TK(BRACK_R, "}")
TK(BRACE_L, "[")
TK(BRACE_R, "]")

TK(SEMI, ";")
TK(COLON, ":")
TK(COMMA, ",")

TK(KW_INT, "int")
TK(KW_CHAR, "char")
TK(KW_SHORT, "short")
TK(KW_LONG, "long")
TK(KW_FLOAT, "float")
TK(KW_DOUBLE, "double")
TK(KW_VOID, "void")

TK(KW_SIGNED, "signed")
TK(KW_UNSIGNED, "unsigned")

TK(KW_STRUCT, "struct")
TK(KW_UNION, "union")
TK(KW_ENUM, "enum")

TK(KW_CONST, "const")
TK(KW_VOLATILE, "volatile")
TK(KW_REGISTER, "register")
TK(KW_RESTRICT, "restrict")

TK(KW_AUTO, "auto")
TK(KW_STATIC, "static")
TK(KW_EXTERN, "extern")
TK(KW_INLINE, "inline")
TK(KW_TYPEDEF, "typedef")

TK(KW_IF, "if")
TK(KW_WHILE, "while")
TK(KW_FOR, "for")
TK(KW_SWITCH, "switch")
TK(KW_DO, "do")
TK(KW_ELSE, "else")
TK(KW_CASE, "case")

TK(KW_BREAK, "break")
TK(KW_RETURN, "return")
TK(KW_CONTINUE, "continue")
TK(KW_GOTO, "goto")

TK(KW_SIZEOF, "sizeof")
TK(KW_COMPLEX, "_Complex")
TK(KW_IMAGINARY, "_Imaginary")

TK(KW_NORETURN, "_Noreturn")

#undef TK
