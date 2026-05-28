#include "da.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define rbdalen(d) (size_t *)((uint8_t *)d - sizeof(size_t[3]))
#define rbdacap(d) (size_t *)((uint8_t *)d - sizeof(size_t[2]))
#define rbdasel(d) (size_t *)((uint8_t *)d - sizeof(size_t[1]))
#define rbda(d) rbdalen(d)

void *i_damk(size_t const sel) {
	void *da									   = malloc(sizeof(size_t[3]) + sel);
	*(size_t *)da								   = 0;
	*(size_t *)((uint8_t *)da + sizeof(size_t))	   = 1;
	*(size_t *)((uint8_t *)da + sizeof(size_t[2])) = sel;
	return (uint8_t *)da + sizeof(size_t[3]);
}

void *darsz(void *da, size_t const nsz) {
	if (nsz == *rbdalen(da)) {
		return da;
	}
	*rbdalen(da) = nsz;
	if (nsz >= *rbdacap(da)) {
		*rbdacap(da) *= 2;
		goto out_update;
	} else if (*rbdacap(da) > 1 && nsz < *rbdacap(da) / 2) {
		*rbdacap(da) /= 2;
		goto out_update;
	}
	return da;
out_update:
	return (uint8_t *)realloc((uint8_t *)da - sizeof(size_t[3]),
							  sizeof(size_t[3]) + (*rbdasel(da) * *rbdacap(da))) +
		   sizeof(size_t[3]);
}

size_t dalen(void const *const da) {
	return *rbdalen(da);
}

void dadel(void *const da) {
	free((uint8_t *)da - sizeof(size_t[3]));
}

void *datrm(void *const da) {
	return (uint8_t *)realloc((uint8_t *)da - sizeof(size_t[3]), dalen(da)) + sizeof(size_t[3]);
}

void *dapsh(void *da, void const *const el) {
	size_t elsz = *rbdasel(da);
	da			= darsz(da, dalen(da) + 1);
	memcpy((uint8_t *)da + ((*rbdalen(da) - 1) * elsz), el, elsz);
	return da;
}

void *dapop(void *da) {
	return darsz(da, dalen(da) - 1);
}
