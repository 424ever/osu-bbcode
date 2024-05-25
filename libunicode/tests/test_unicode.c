#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "test.h"
#include "uc.h"

static void dump(uc_codepoint *s, size_t c)
{
	size_t i;
	for (i = 0; i < c; ++i)
	{
		fprintf(stderr, "[%lu] %c %x\n", (unsigned long) i,
			s[i].err ? 'X' : ' ', s[i].code);
	}
}

int main(void)
{
	char	     *s1;
	uc_codepoint *u1;
	uc_codepoint *u2;

	fprintf(stderr, " =====> TESTS UNICODE\n");

	test_assert_cp_ok("from_ascii success", uc_from_ascii('A'));
	test_assert_cp_err("from_ascii fail", uc_from_ascii((char) 128));

	test_assert("is_ascii in range", uc_is_ascii(make_cp(0, 69)));
	test_assert("is_ascii out of range", !uc_is_ascii(make_cp(0, 128)));
	test_assert("is_ascii error", !uc_is_ascii(make_cp(1, 0)));

	test_assert("is_err yes", uc_is_err(uc_from_ascii((char) 128)));
	test_assert("is_err no", !uc_is_err(uc_from_ascii('A')));

	test_assert("is_nul yes", uc_is_nul(uc_from_ascii('\0')));
	test_assert("is_nul no", !uc_is_nul(uc_from_ascii('A')));

	test_assert("make_nu", uc_is_nul(uc_make_nul()));

	u1 = uc_from_ascii_str("ABC");
	test_assert("from_ascii_str valid nonnull", u1 != NULL);
	u2    = calloc(4, sizeof(uc_codepoint));
	u2[0] = make_cp(0, 65);
	u2[1] = make_cp(0, 66);
	u2[2] = make_cp(0, 67);
	u2[3] = uc_make_nul();
	test_assert("from_ascii_str valid correct", uc_strcmp(u1, u2) == 0);
	free(u1);
	free(u2);

	u1 = uc_from_ascii_str("ÄB");
	test_assert("from_ascii_str invalid null", u1 == NULL);

	u1 = uc_from_ascii_str("ABC");
	s1 = uc_to_ascii_str(u1);
	test_assert("to_ascii_str", !strcmp("ABC", s1));
	free(s1);
	free(u1);

	u1 = uc_from_ascii_str("ABC");
	test_assert("strlen", uc_strlen(u1) == 3);
	free(u1);

	u1 = uc_from_ascii_str("ABC");
	u2 = uc_from_ascii_str("ABC");
	test_assert("strcmp eq", uc_strcmp(u1, u2) == 0);
	free(u2);
	u2 = uc_from_ascii_str("AB");
	test_assert("strcmp neq", uc_strcmp(u1, u2) > 0);
	free(u2);
	free(u1);

	test_end();
}
