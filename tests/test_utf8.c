#include <stdio.h>

#include "test.h"

int main(void)
{
	FILE	 *f;
	size_t	  c;
	uc_string u1;
	uc_string u2;

	fprintf(stderr, " =====> TESTS UTF-8\n");

	f = fmemopen("\x41", 1, "r");
	test_assert_cp_eq("single byte", utf8_read_codepoint(f),
			  make_cp(0, 0x41));
	fclose(f);

	f = fmemopen("\xc3\x84", 2, "r");
	test_assert_cp_eq("two bytes", utf8_read_codepoint(f),
			  make_cp(0, 0xc4));
	fclose(f);

	f = fmemopen("\xe0\xa4\x86", 3, "r");
	test_assert_cp_eq("three bytes", utf8_read_codepoint(f),
			  make_cp(0, 0x0906));
	fclose(f);

	f = fmemopen("\xf0\x91\x96\x80", 4, "r");
	test_assert_cp_eq("four bytes", utf8_read_codepoint(f),
			  make_cp(0, 0x11580));
	fclose(f);

	f = fmemopen("\xff", 2, "r");
	test_assert_cp_err("invalid header", utf8_read_codepoint(f));
	fclose(f);

	f = fmemopen("\xc3\xff", 2, "r");
	test_assert_cp_err("invalid second byte", utf8_read_codepoint(f));
	fclose(f);

	f = fmemopen("", 0, "r");
	c = utf8_read_file(f, &u1);
	test_assert("empty file 0 count", c == 0);
	fclose(f);
	uc_string_free(u1);

	f  = fmemopen("\x41", 1, "r");
	c  = utf8_read_file(f, &u1);
	u2 = uc_from_ascii_str("A");
	test_assert("one point count", c == 1);
	test_assert_us_eq("one point eq", u2, u1);
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	f = fmemopen("\x68\xc3\xa4\x6c\x6c\x6f\x2c\x20\x77\xc3\xb6\x72\x6c\x64",
		     14, "r");
	c = utf8_read_file(f, &u1);
	u2 = uc_from_ascii_str("hello, world");
	uc_string_set(u2, 1, make_cp(0, 0xe4));
	uc_string_set(u2, 8, make_cp(0, 0xf6));
	test_assert_eq_i("complex count", c, 12);
	test_assert_us_eq("complex eq", u2, u1);
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	test_end();
}
