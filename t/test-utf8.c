#include <stdio.h>

#include "./common.h"
#include "error.h"
#include "tap.h"
#include "unicode.h"
#include "utf8.h"

int main(void)
{
	FILE	 *f;
	uc_string u1;
	uc_string u2;

	plan(NO_PLAN);

	f = fmemopen("\x41", 1, "r");
	unset_error();
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0x41)), "single byte");
	no_error("single byte error");
	fclose(f);

	f = fmemopen("\xc3\x84", 2, "r");
	unset_error();
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0xc4)), "two bytes");
	no_error("two bytes error");
	fclose(f);

	f = fmemopen("\xe0\xa4\x86", 3, "r");
	unset_error();
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0x0906)), "three bytes");
	no_error("three bytes error");
	fclose(f);

	f = fmemopen("\xf0\x91\x96\x80", 4, "r");
	unset_error();
	ok(uc_eq(utf8_read_codepoint(f), make_cp(0x11580)), "four bytes");
	no_error("four bytes error");
	fclose(f);

	f = fmemopen("\xff", 2, "r");
	unset_error();
	(void) utf8_read_codepoint(f);
	yes_error("invalid header");
	fclose(f);

	f = fmemopen("\xc3\xff", 2, "r");
	(void) utf8_read_codepoint(f);
	yes_error("invalid second byte");
	fclose(f);

	f = fmemopen("", 0, "r");
	unset_error();
	u1 = utf8_read_file(f);
	cmp_ok(uc_strlen(u1), "==", 0, "empty file count");
	no_error("empty file error");
	fclose(f);
	uc_string_free(u1);

	f = fmemopen("\x41", 1, "r");
	unset_error();
	u1 = utf8_read_file(f);
	u2 = uc_from_ascii_str("A");
	cmp_ok(uc_strlen(u1), "==", 1, "one point count");
	ok(!uc_strcmp(u1, u2), "one point eq");
	no_error("one point error");
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	f = fmemopen("\x68\xc3\xa4\x6c\x6c\x6f\x2c\x20\x77\xc3\xb6\x72\x6c\x64",
		     14, "r");
	unset_error();
	u1 = utf8_read_file(f);
	u2 = uc_from_ascii_str("hello, world");
	uc_string_set(u2, 1, make_cp(0xe4));
	uc_string_set(u2, 8, make_cp(0xf6));
	cmp_ok(uc_strlen(u1), "==", 12, "complex count");
	ok(!uc_strcmp(u1, u2), "complex eq");
	no_error("complex error");
	fclose(f);
	uc_string_free(u1);
	uc_string_free(u2);

	done_testing();
}
