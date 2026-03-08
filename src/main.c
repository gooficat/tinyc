#include "tok.h"

int main(void)
{
	tokstrm ts;
	tokstrminit(&ts, "../tests/test1");
	while (*ts.tbuf)
	{
		printf("'%s'\n", ts.tbuf);
		tokstrmnext(&ts);
	}
}