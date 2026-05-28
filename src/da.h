#ifndef __DA__H__
#define __DA__H__

#include <stddef.h>

void *idamk(size_t const sel);

void  *darsz(void *const da, size_t const nsz);
size_t dalen(void const *const da);
void   dadel(void *const da);
void  *datrm(void *const da);
void  *dapsh(void *da, void const *const el);
void  *dapop(void *da);

#define damk(t) ((t *)idamk(sizeof(t)))

#endif
