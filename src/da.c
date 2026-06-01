#include "da.h"
#include <malloc.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>

void *idamk(size_t const sel) {
	size_t *da = (size_t *)malloc(sizeof(size_t[2]) + sel) + 2;
	da[-2]	   = 0;
	da[-1]	   = 1;
	return da;
}

void *idatrm(size_t *const da, size_t const sel) {
	if (da[-2] == da[-1]) {
		return da;
	} else {
		return (size_t *)realloc(da - 2, sizeof(size_t[2]) + da[-2] * sel) + 2;
	}
}

void *idarsz(size_t *da, size_t const nsz, size_t const sel) {
	if (nsz == da[-2]) {
		return da;
	}

	if (nsz > da[-1]) {
		do {
			da[-1] *= 2;
		} while (nsz > da[-1]);
		da = (size_t *)realloc(da - 2, sizeof(size_t[2]) + da[-1] * sel) + 2;
	} else {
		while (da[-1] > 1 && nsz < da[-1] / 2) {
			da[-1] /= 2;
		}
		da = (size_t *)realloc(da - 2, sizeof(size_t[2]) + da[-1] * sel) + 2;
	}
	da[-2] = nsz;
	return da;
}

void *idapsh(size_t *da, void const *const el, size_t const sel) {
	da = idarsz(da, da[-2] + 1, sel);
	memcpy((uint8_t *)da + ((da[-2] - 1) * sel), el, sel);
	return da;
}

void *idapop(size_t *da, size_t const sel) {
	da = idarsz(da, da[-2] - 1, sel);
	return da;
}
