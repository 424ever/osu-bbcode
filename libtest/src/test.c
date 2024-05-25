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
