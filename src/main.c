#include "token.h"
#include <stdio.h>

int main(void)
{
	TokenStream stream;
	TokenStreamInit(&stream, "./tests/test1", 512);
	TokenStreamNext(&stream);

	while (*stream.token)
	{
		printf("%s\n", stream.token);
		TokenStreamNext(&stream);
	}

	return 0;
}
