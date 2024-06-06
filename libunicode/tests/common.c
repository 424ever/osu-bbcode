#include <stdio.h>

#include "common.h"
#include "test.h"
#include "uc.h"

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
