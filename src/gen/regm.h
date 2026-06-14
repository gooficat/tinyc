#ifndef __REGM__H__
#define __REGM__H__

// x64 btw

// register tracking machine

typedef struct {
	char *name; // TODO!!!! smaller registers
	bool is_available;
	bool is_volatile;
	bool is_floating;
} Register;

extern Register REGISTERS[];

#endif //!__REGM__H__
