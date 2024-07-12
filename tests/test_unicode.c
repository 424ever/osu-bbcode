#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

int main(void)
{
	char	 *s1;
	uc_string u1;
	uc_string u2;

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
	test_assert_no_error("from_ascii_str no error");
	u2 = uc_string_new(3);
	uc_string_set(u2, 0, make_cp(0, 65));
	uc_string_set(u2, 1, make_cp(0, 66));
	uc_string_set(u2, 2, make_cp(0, 67));
	test_assert("from_ascii_str valid correct", uc_strcmp(u1, u2) == 0);
	uc_string_free(u1);
	uc_string_free(u2);

	u1 = uc_from_ascii_str("ÄB");
	test_assert_error("from_ascii_str error");
	uc_string_free(u1);

	u1 = uc_from_ascii_str("ABC");
	s1 = uc_to_ascii_str(u1);
	test_assert("to_ascii_str", !strcmp("ABC", s1));
	uc_string_free(u1);
	free(s1);

	u1 = uc_from_ascii_str("ABC");
	test_assert("strlen", uc_strlen(u1) == 3);
	uc_string_free(u1);

	u1 = uc_from_ascii_str("ABC");
	u2 = uc_from_ascii_str("ABC");
	test_assert("strcmp eq", uc_strcmp(u1, u2) == 0);
	uc_string_free(u2);
	u2 = uc_from_ascii_str("AB");
	test_assert("strcmp neq", uc_strcmp(u1, u2) > 0);
	uc_string_free(u1);
	uc_string_free(u2);

	test_end();
}
