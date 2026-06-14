#include "error.h"
#include <stdio.h>

noreturn void error(char const *msg) {
	fprintf(stderr, "ERROR!\n%s\n", msg);
	// I place a breakpoint here
	exit(EXIT_FAILURE);
}
