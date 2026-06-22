char line[4096] = {'\n'};
char *scan = line;

lexnext() {
rpt:
	if (*scan == '\n') {
		gets(line);
		scan = line;
		goto rpt;
	}
	if (!*scan)
		return -1; /*eof*/
	if (isspace(*scan)) {
		++scan;
		goto rpt;
	}
	if (*scan == '"')
		return prsstr(); /*-2*/
	if (isdigit(*scan))
		return prsnum(); /*-3 if int, -4 if float*/
	return prswrd();
}

prswrd() {
	extern char *toks[];
	auto i = 0;
	while (toks[i]) {
		auto len = strlen(toks[i]);
		if (!memcmp(scan, toks[i], len) && /**/
			(!isalnum(scan[len - 1]) || !isalnum(scan[len])))
			return i;
		++i;
	}
	return prssym();
}
