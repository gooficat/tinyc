char **syms = (void *)0;
n_syms = 0;
char **strs = (void *)0;
n_strs = 0;
num;
double flt;

extern char *scan;

prsstr() {
	auto i = 0;
	char *str;
	++scan;
	while (scan[i] != '"' || scan[i - 1] == '\\')
		++i;
	str = malloc(i + 1);
	memcpy(str, scan, i);
	str[i] = 0;
	strs = realloc(strs, ++n_strs);
	strs[n_strs - 1] = str;
	return -2; /*str*/
}

prsnum() {
	/*TODO floats*/
	num = strtol(scan, &scan, 0);
	return -3; /*int*/
}

prssym() {
	auto i = 0;
	char *sym;
	++scan;
	while (scan[i] != '"' || scan[i - 1] == '\\')
		++i;
	sym = malloc(i + 1);
	memcpy(sym, scan, i);
	sym[i] = 0;
	syms = realloc(strs, ++n_syms);
	syms[n_syms - 1] = sym;
	return -5; /*sym*/
}
