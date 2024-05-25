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

void test_assert_us_eq(const char *name, const uc_codepoint *a,
		       const uc_codepoint *b, size_t n)
{
	size_t i;

	if (uc_memcmp(a, b, n))
	{
		test_fail(name);
		fprintf(stderr, "expected\tgot\n");
		for (i = 0; i < n; ++i)
		{
			fprintf(stderr, "%c 0x%x\t\t%c 0x%x\n",
				a[i].err ? 'E' : ' ', a[i].code,
				b[i].err ? 'E' : ' ', b[i].code);
		}
		fprintf(stderr, "message: %s\n", uc_last_error());
	}
	else
		test_pass();
}
