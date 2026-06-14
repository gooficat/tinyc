#include "error.h"
#include <stdio.h>

noreturn void error(char const *msg) {
	fprintf(stderr, "ERROR!\n%s\n", msg);
	exit(EXIT_FAILURE);
}
