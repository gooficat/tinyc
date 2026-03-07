#pragma once
#include <stdlib.h>

#define vecStruct(t, allocatorT)                                                                                       \
	{                                                                                                                  \
		t *data;                                                                                                       \
		allocatorT size;                                                                                               \
		allocatorT capacity;                                                                                           \
	}

#define mVecInit(vec)                                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		vec.size = 0;                                                                                                  \
		vec.capacity = 1;                                                                                              \
		vec.data = malloc(sizeof(*vec.data));                                                                          \
	} while (0)

#define vecDestroyM(vec) free(vec.data)

#define mVecPush(vec, val)                                                                                             \
	do                                                                                                                 \
	{                                                                                                                  \
		if (vec.size >= vec.capacity)                                                                                  \
		{                                                                                                              \
			vec.capacity *= 2;                                                                                         \
			vec.data = realloc(vec.data, vec.capacity * sizeof(*vec.data));                                            \
		}                                                                                                              \
		vec.data[vec.size++] = val;                                                                                    \
	} while (0)

#define mVecPop(vec)                                                                                                   \
	do                                                                                                                 \
	{                                                                                                                  \
		if (--vec.size < vec.capacity / 2)                                                                             \
		{                                                                                                              \
			vec.capacity /= 2;                                                                                         \
			vec.data = realloc(vec.data, vec.capacity * sizeof(*vec.data));                                            \
		}                                                                                                              \
	} while (0)

#define vecInit(vec) mVecInit((vec))
#define vecFree(vec) free((vec).data)
#define vecPush(vec, val) mVecPush((vec), (val))
#define vecPop(vec) mVecPop((vec))
