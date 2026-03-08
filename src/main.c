#include "tok.h"

int main(void)
{
	tokstrm ts;
	tokstrminit(&ts, "../tests/test1");
	while (*ts.tbuf)
	{
		printf("'%s'", ts.tbuf);
		tokstrmnext(&ts);
	}
}