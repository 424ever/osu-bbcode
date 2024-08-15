#include "error.h"
#include "tap.h"
#include "unicode.h"

uc_codepoint make_cp(uint32_t code)
{
	uc_codepoint c;
	c.code = code;
	return c;
}

void uis_n(uc_string got, uc_string expected, const char *m, const char *name)
{
	int test;

	test = !uc_strcmp(got, expected);
	ok(test, m, name);
	if (!test)
	{
		diag("         got: '%s'", uc_to_ascii_str(got));
		diag("    expected: '%s'", uc_to_ascii_str(expected));
	}
}

void uis(uc_string got, uc_string expected, const char *m)
{
	int test;

	test = !uc_strcmp(got, expected);
	ok(test, m);
	if (!test)
	{
		diag("         got: '%s'", uc_to_ascii_str(got));
		diag("    expected: '%s'", uc_to_ascii_str(expected));
	}
}

void no_error(const char *name)
{
	ok(!error_occured(), name);
}

void yes_error(const char *name)
{
	ok(error_occured(), name);
}
