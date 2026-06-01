#ifndef __DA__H__
#define __DA__H__

#include <malloc.h>
#include <stddef.h>

void *idamk(size_t const sel);

void *idarsz(size_t *da, size_t const nsz, size_t const sel);
void *idatrm(size_t *const da, size_t const sel);
void *idapsh(size_t *da, void const *const el, size_t const sel);
void *idapop(size_t *da, size_t const sel);

#define damk(t) ((t *)idamk(sizeof(t)))
#define darsz(da, nsz) idarsz((size_t *)(da), (nsz), (sizeof *(da)))
#define dalen(da) (((size_t *)(da))[-2])
#define dadel(da) free(((size_t *)(da)) - 2)
#define datrm(da) idatrim((size_t *)(da), (sizeof *(da)))
#define dapsh(da, el) idapsh((size_t *)(da), (el), (sizeof *(da)))
#define dapop(da) idapop((size_t *)(da), (sizeof *(da)))

#endif
