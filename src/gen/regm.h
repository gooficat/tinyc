#ifndef __REGM__H__
#define __REGM__H__

#include <stdbool.h>

// x86-32 btw

// register tracking machine

#define WORD_WIDTH 4

typedef struct {
	char *name; // TODO!!!! smaller registers
	bool is_available;
	bool is_volatile;
	bool is_floating;
} Register;

extern Register REGISTERS[];
extern Register *CDECL_PARAMS[];

Register *take_register(bool floating);

#endif //!__REGM__H__
