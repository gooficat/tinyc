#include "main.h"
#include <string.h>
#include <stdlib.h>

size_t intrin_sizeof(struct ctype *type)
{
	switch (type->type)
	{
	case CTYPE_ERR:
		// error
		exit(EXIT_FAILURE);
	case CTYPE_COM:
		return intrin_sizeof(type->val.com.under) * type->val.com.num;
	case CTYPE_NON:
		// error
		exit(EXIT_FAILURE);
	case CTYPE_INT:
		return 4; // TODO
	case CTYPE_FLT:
		return 4; // TODO
	case CTYPE_PTR:
		return 8; // TODO
	}
}

void find_sym(char *name, struct csym_table *tbl, struct mem_desc *dsc)
{
	for (size_t i = 0; i < tbl->syms.len; ++i)
	{
		if (!strcmp(name, tbl->syms.els[i].name))
		{
			dsc->type = tbl->syms.els[i].mem; // TODO
		}
		if (tbl->syms.els[i].mem == MEM_STK)
		{
			dsc->val += intrin_sizeof(tbl->syms.els[i].type);
		}
	}
}
