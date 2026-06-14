#ifndef __GEN__H__
#define __GEN__H__

#include "parse/ast.h"
#include "regm.h"
#include <stdint.h>
#include <stdio.h>

typedef enum {
	MEM_EXTERN,
	MEM_STATIC,
	MEM_STACK,
	MEM_REG,
} MemType;

typedef struct {
	MemType type;
	union {
		char const *name;
		int32_t stack;
		Register *reg;
	};
} MemLoc;

void codegen_tree();
void codegen_init(FILE *file);

#endif //!__GEN__H__
