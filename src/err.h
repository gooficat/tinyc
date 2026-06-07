#ifndef __ERR__H__
#define __ERR__H__

void parse_panic(const char *msg);
void codegen_panic(const char *msg);

#define dbg_print(buf) fputs(buf, stdout)

void tree_print(void);

#endif
