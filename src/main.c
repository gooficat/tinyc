#include "main.h"

int main(void)
{
    do
    {
        lex_next();
        printf("%i\n", tok.type);
    } while (tok.type != TOK_EOF);
    return 0;
}
