#include "types.h"

type INBUILT_TYPES[] = {
	{TYPE_INT, 1, {{0}}}, // uchar
	{TYPE_INT, 1, {{1}}}, // schar
	{TYPE_INT, 2, {{0}}}, // ushort
	{TYPE_INT, 2, {{1}}}, // sshort
	{TYPE_INT, 4, {{0}}}, // uint
	{TYPE_INT, 4, {{1}}}, // sint
	{TYPE_INT, 8, {{0}}}, // ulong
	{TYPE_INT, 8, {{1}}}, // slong

	{TYPE_FLOAT, 4, {{0}}},	 // float
	{TYPE_FLOAT, 8, {{0}}},	 // double
	{TYPE_FLOAT, 16, {{0}}}, // long double

	{TYPE_ERR, 0, {{0}}}, // void
};
