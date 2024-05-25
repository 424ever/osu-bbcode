#include <string.h>

#include "test.h"
#include "uc.h"
#include "unicode.h"

int main(int argc, char *argv[])
{
	fprintf(stderr, " =====> TESTS ERROR\n");

	test_assert("empty at start", !strcmp(uc_last_error(), ""));

	(void) uc_from_ascii((char) 128);
	test_assert("set after error", strcmp(uc_last_error(), "") > 0);

	uc_unset_error_();
	test_assert("empty after unset", !strcmp(uc_last_error(), ""));

	test_end();
}
