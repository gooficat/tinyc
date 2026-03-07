#include "astgen.h"

void ASTGenTree(ASTGlobal *global, TokenStream *stream)
{
	vecInit(global->functions);
	while (*stream->token)
	{
		printf("%s\n", stream->token);
		TokenStreamNext(stream);
	}
}
