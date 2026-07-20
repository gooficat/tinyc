#include <stdio.h>
extern char const * const tok_strs[];

int main(void) {
  char const * const *p = tok_strs;
  while (*p) {
    printf("%s\n", *p);
    p++;
  }
  return 0;
}
