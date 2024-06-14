#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "unicode.h"

int passcount = 0;
int failcount = 0;

void test_assert(const char *name, int cond)
{
	if (!cond)
		test_fail(name);
	else
		test_pass(name);
}

void test_assert_eq_i(const char *name, int a, int b)
{
	if (a != b)
	{
		test_fail(name);
		fprintf(stderr, "  %d != %d\n", a, b);
	}
	else
		test_pass(name);
}

void test_end(void)
{
	fprintf(stderr, "%d tests passed, %d failed\n\n", passcount, failcount);
	if (failcount > 0)
		exit(10);
	else
		exit(0);
}

void test_fail(const char *name)
{
	fprintf(stderr, "[\033[31;1mFAIL\033[0m] %s\n", name);
	++failcount;
}

void test_pass(const char *name)
{
	fprintf(stderr, "[\033[32;1mPASS\033[0m] %s\n", name);
	++passcount;
}

void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b)
{
	test_assert(name, !(a.err != b.err || a.code != b.code));
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
	test_assert(name, !c.err);
}

void test_assert_cp_err(const char *name, uc_codepoint c)
{
	test_assert(name, c.err);
}

void test_assert_us_eq(const char *name, const uc_codepoint *a,
		       const uc_codepoint *b, size_t n)
{
	test_assert(name, !uc_memcmp(a, b, n));
}
