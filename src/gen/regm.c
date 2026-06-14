#include "regm.h"
#include "parse/type.h"

Register REGISTERS[] = {
	{
		"eax",
		true,
		true,
		false,
	},
	{
		"ecx",
		true,
		true,
		false,
	},
	{
		"edx",
		true,
		true,
		false,
	},
	{
		"ebx",
		true,
		false,
		false,
	},
	{
		"esp",
		false,
		false,
		false,
	},
	{
		"ebp",
		false,
		false,
		false,
	},
	{
		"esi",
		true,
		false,
		false,
	},
	{
		"edi",
		true,
		false,
		false,
	},
	{
		"xmm0",
		true,
		true,
		true,
	},
	{
		"xmm1",
		true,
		true,
		true,
	},
	{
		"xmm2",
		true,
		true,
		true,
	},
	{
		"xmm3",
		true,
		true,
		true,
	},
	{
		"xmm4",
		true,
		true,
		true,
	},
	{
		"xmm5",
		true,
		true,
		true,
	},
	{
		"xmm6",
		true,
		true,
		true,
	},
	{
		"xmm7",
		true,
		true,
		true,
	},
};

Register *take_register(bool floating) {
	for (size_t i = 0; i < sizeof(REGISTERS) / sizeof(Register); ++i)
		if (!REGISTERS[i].is_volatile && REGISTERS[i].is_available && (!floating || REGISTERS[i].is_floating))
			return &REGISTERS[i];
	for (size_t i = 0; i < sizeof(REGISTERS) / sizeof(Register); ++i)
		if (REGISTERS[i].is_available && (!floating || REGISTERS[i].is_floating))
			return &REGISTERS[i];
	return NULL;
}

void close_register(Register *reg) {
	reg->is_available = false;
}
