#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "uc.h"

int passcount = 0;
int failcount = 0;

void test_assert(const char *name, int cond)
{
	if (!cond)
	{
		fprintf(stderr, "Test '%s' failed.\n", name);
		++failcount;
	}
	else
		++passcount;
}

void test_end(void)
{
	fprintf(stderr, "%d tests passed, %d failed\n\n", passcount, failcount);
	if (failcount > 0)
		exit(10);
	else
		exit(0);
}

void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b)
{
	if (a.err != b.err || a.code != b.code)
	{
		fprintf(stderr, "Test '%s' failed. [%c, 0x%x] != [%c, 0x%x]\n",
			name, a.err ? 'E' : ' ', a.code, b.err ? 'E' : ' ',
			b.code);
		++failcount;
	}
	else
		++passcount;
}

uc_codepoint make_cp(int err, uint32_t code)
{
	uc_codepoint c;
	c.err  = err;
	c.code = code;
	return c;
}
