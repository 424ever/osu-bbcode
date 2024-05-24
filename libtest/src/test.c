#include <stdio.h>
#include <stdlib.h>

#include "test.h"

int passcount = 0;
int failcount = 0;

void test_assert(const char *name, int cond)
{
	if (!cond)
		test_fail(name);
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

void test_fail(const char *name)
{
	fprintf(stderr, "Test '%s' failed.\n", name);
	++failcount;
}

void test_pass(void)
{
	++passcount;
}
