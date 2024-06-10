#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "test.h"
#include "uc.h"

int main(void)
{
	FILE		   *f;
	size_t		    c;
	uc_codepoint	   *u1;
	uc_codepoint	   *u2;
	struct alloc_arena *a;

	a = arena_new();

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

	f  = fmemopen("", 0, "r");
	u1 = utf8_read_file(f, &c, a);
	test_assert("empty file nonnull", u1 != NULL);
	test_assert("empty file 0 count", c == 0);
	fclose(f);

	f  = fmemopen("\x41", 1, "r");
	u1 = utf8_read_file(f, &c, a);
	u2 = uc_from_ascii_str("A", a);
	test_assert("one point nonnull", u1 != NULL);
	test_assert("one point count", c == 1);
	test_assert_us_eq("one point eq", u2, u1, 1);
	fclose(f);

	f = fmemopen(
	    "\x48\xc3\xa4\x6c\x6c\x6f\x2c\x20\x77\xc3\xb6\x72\x6c\x64\x21", 15,
	    "r");
	u1	   = utf8_read_file(f, &c, a);
	u2	   = uc_from_ascii_str("Hello, world!", a);
	u2[1].code = 0xe4;
	u2[8].code = 0xf6;
	test_assert("complex nonnull", u1 != NULL);
	test_assert("complex count", c == 13);
	test_assert_us_eq("compley eq", u2, u1, 13);
	fclose(f);

	arena_destroy(a);

	test_end();
}
