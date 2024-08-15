#include <stdint.h>
#include <stdlib.h>

#include "./common.h"
#include "error.h"
#include "tap.h"
#include "unicode.h"

int main(void)
{
	char	 *s1;
	uc_string u1;
	uc_string u2;

	plan(NO_PLAN);

	unset_error();
	(void) uc_from_ascii('A');
	ok(!error_occured(), "from_ascii success");
	unset_error();
	(void) uc_from_ascii(128);
	ok(error_occured(), "from_ascii fail");

	ok(uc_is_ascii(make_cp(69)), "is_ascii in range");
	ok(!uc_is_ascii(make_cp(128)), "is_ascii out of range");

	unset_error();
	u1 = uc_from_ascii_str("ABC");
	no_error("from_ascii_str no error");
	u2 = uc_string_new(3);
	uc_string_set(u2, 0, make_cp(65));
	uc_string_set(u2, 1, make_cp(66));
	uc_string_set(u2, 2, make_cp(67));
	ok(!uc_strcmp(u1, u2), "from_ascii_str valid correct");
	uc_string_free(u1);
	uc_string_free(u2);

	unset_error();
	u1 = uc_from_ascii_str("ÄB");
	yes_error("from_ascii_str error");
	uc_string_free(u1);

	u1 = uc_from_ascii_str("ABC");
	s1 = uc_to_ascii_str(u1);
	is(s1, "ABC", "to_ascii_str");
	uc_string_free(u1);
	free(s1);

	u1 = uc_from_ascii_str("ABC");
	cmp_ok(uc_strlen(u1), "==", 3, "strlen");
	uc_string_free(u1);

	u1 = uc_from_ascii_str("ABC");
	u2 = uc_from_ascii_str("ABC");
	ok(!uc_strcmp(u1, u2), "strcmp eq");
	uc_string_free(u2);
	u2 = uc_from_ascii_str("AB");
	ok(uc_strcmp(u1, u2), "strcmp neq");
	uc_string_free(u1);
	uc_string_free(u2);

	u1 = uc_from_ascii_str("A");
	u2 = uc_from_ascii_str("A");
	uis(u1, u2, "append 1");
	uc_string_free(u2);
	uc_string_append(u1, uc_from_ascii('B'));
	u2 = uc_from_ascii_str("AB");
	uis(u1, u2, "append 2");
	uc_string_free(u2);
	uc_string_append(u1, uc_from_ascii('C'));
	uc_string_append(u1, uc_from_ascii('D'));
	u2 = uc_from_ascii_str("ABCD");
	uis(u1, u2, "append 3");
	uc_string_free(u2);
	uc_string_free(u1);

	done_testing();
}
