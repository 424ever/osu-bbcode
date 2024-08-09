#include <stdio.h>

#include "tap.h"
#include "unicode.h"
#include "utf8.h"

uc_codepoint make_cp(int err, uint32_t code)
{
	uc_codepoint c;
	c.err  = err;
	c.code = code;
	return c;
}

int main(void)
{
	FILE	 *f;
	size_t	  c;
	uc_string u1;
	uc_string u2;

	plan(NO_PLAN);

	f = fmemopen("\x41", 1, "r");
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0, 0x41)), "single byte");
	fclose(f);

	f = fmemopen("\xc3\x84", 2, "r");
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0, 0xc4)), "two bytes");
	fclose(f);

	f = fmemopen("\xe0\xa4\x86", 3, "r");
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0, 0x0906)), "three bytes");
	fclose(f);

	f = fmemopen("\xf0\x91\x96\x80", 4, "r");
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0, 0x11580)), "four bytes");
	fclose(f);

	f = fmemopen("\xff", 2, "r");
	ok(uc_is_err(utf8_read_codepoint(f)), "invalid header");
	fclose(f);

	f = fmemopen("\xc3\xff", 2, "r");
	ok(uc_is_err(utf8_read_codepoint(f)), "invalid second byte");
	fclose(f);

	f = fmemopen("", 0, "r");
	c = utf8_read_file(f, &u1);
	cmp_ok(c, "==", 0, "empty file count");
	fclose(f);
	uc_string_free(u1);

	f  = fmemopen("\x41", 1, "r");
	c  = utf8_read_file(f, &u1);
	u2 = uc_from_ascii_str("A");
	cmp_ok(c, "==", 1, "one point count");
	ok(!uc_strcmp(u1, u2), "one point eq");
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	f = fmemopen("\x68\xc3\xa4\x6c\x6c\x6f\x2c\x20\x77\xc3\xb6\x72\x6c\x64",
		     14, "r");
	c = utf8_read_file(f, &u1);
	u2 = uc_from_ascii_str("hello, world");
	uc_string_set(u2, 1, make_cp(0, 0xe4));
	uc_string_set(u2, 8, make_cp(0, 0xf6));
	cmp_ok(c, "==", 12, "complex count");
	ok(!uc_strcmp(u1, u2), "complex eq");
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	done_testing();
}
