#include "astgen.h"
#include "token.h"

int main(void)
{
	TokenStream stream;
	TokenStreamInit(&stream, "./tests/test1", 512);
	TokenStreamNext(&stream);

	ASTGlobal tree;
	ASTGenTree(&tree, &stream);

	return 0;
}
