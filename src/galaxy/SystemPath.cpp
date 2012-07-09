#include "SystemPath.h"
#include <cstdlib>

static int ParseInt(const char *&ss)
{
	assert(ss);
	char *end = 0;
	long n = strtol(ss, &end, 10);
	if (ss == end) {
		throw SystemPath::ParseFailure();
	} else {
		ss = end;
		return n;
	}
}

SystemPath SystemPath::Parse(const char * const str)
{
	assert(str);

	// syspath = '('? [+-]? [0-9]+ [, +-] [0-9]+ [, +-] [0-9]+ ([, +-] [0-9]+)? ')'?
	// with whitespace allowed between tokens

	const char *s = str;

	int x, y, z, si = -1;

	while (isspace(*s)) { ++s; }
	if (*s == '(') { ++s; }

	x = ParseInt(s); // note: ParseInt (actually, strtol) skips leading whitespace itself

	while (isspace(*s)) { ++s; }
	if (*s == ',' || *s == '.') { ++s; }

	y = ParseInt(s);

	while (isspace(*s)) { ++s; }
	if (*s == ',' || *s == '.') { ++s; }

	z = ParseInt(s);

	while (isspace(*s)) { ++s; }

	// system index
	if (*s == ',' || *s == '.') {
		++s;
		si = ParseInt(s);
		while (isspace(*s)) { ++s; }
	}

	if (*s == ')') { ++s; }
	while (isspace(*s)) { ++s; }

	if (*s) // extra unexpected text after the system path
		throw SystemPath::ParseFailure();
	else if (si >= 0)
		return SystemPath(x, y, z, si);
	else
		return SystemPath(x, y, z);
}
