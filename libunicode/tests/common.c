#define _XOPEN_SOURCE 500
#include <stdio.h>

#include "common.h"
#include "test.h"
#include "uc.h"

void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b)
{
	char newname[128];

	if (a.err != b.err || a.code != b.code)
	{
		snprintf(newname, 128, "%s([%c, 0x%x] == [%c, 0x%x])", name,
			 a.err ? 'E' : ' ', a.code, b.err ? 'E' : ' ', b.code);
		test_fail(newname);
	}
	else
		test_pass();
}

uc_codepoint make_cp(int err, uint32_t code)
{
	uc_codepoint c;
	c.err  = err;
	c.code = code;
	return c;
}

void test_assert_cp_ok(const char *name, uc_codepoint c)
{
	if (c.err)
		test_fail(name);
	else
		test_pass();
}

void test_assert_cp_err(const char *name, uc_codepoint c)
{
	if (!c.err)
		test_fail(name);
	else
		test_pass();
}
