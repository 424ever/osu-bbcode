#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "test.h"
#include "uc.h"

static uint8_t buf[6];

static FILE *init_f(unsigned c, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
		    uint8_t b4, uint8_t b5)
{
	buf[0] = b0;
	buf[1] = b1;
	buf[2] = b2;
	buf[3] = b3;
	buf[4] = b4;
	buf[5] = b5;

	return fmemopen(buf, c, "r");
}

int main(void)
{
	FILE	     *f;
	size_t	      c;
	uc_codepoint *u1;
	uc_codepoint *u2;

	fprintf(stderr, " =====> TESTS UTF-8\n");

	f = init_f(1, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00);
	test_assert_cp_eq("single byte", utf8_read_codepoint(f),
			  make_cp(0, 0x41));
	fclose(f);

	f = init_f(2, 0xc3, 0x84, 0x00, 0x00, 0x00, 0x00);
	test_assert_cp_eq("two bytes", utf8_read_codepoint(f),
			  make_cp(0, 0xc4));
	fclose(f);

	f = init_f(3, 0xe0, 0xa4, 0x86, 0x00, 0x00, 0x00);
	test_assert_cp_eq("three bytes", utf8_read_codepoint(f),
			  make_cp(0, 0x0906));
	fclose(f);

	f = init_f(4, 0xf0, 0x91, 0x96, 0x80, 0x00, 0x00);
	test_assert_cp_eq("four bytes", utf8_read_codepoint(f),
			  make_cp(0, 0x11580));
	fclose(f);

	f = init_f(2, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
	test_assert_cp_err("invalid header", utf8_read_codepoint(f));
	fclose(f);

	f = init_f(2, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x00);
	test_assert_cp_err("invalid second byte", utf8_read_codepoint(f));
	fclose(f);

	f  = fmemopen("", 0, "r");
	u1 = utf8_read_file(f, &c);
	test_assert("empty file nonnull", u1);
	test_assert("empty file 0 count", c == 0);
	free(u1);
	fclose(f);

	f  = fmemopen("\x41", 1, "r");
	u1 = utf8_read_file(f, &c);
	u2 = uc_from_ascii_str("A");
	test_assert("one point nonnull", u1);
	test_assert("one point count", c == 1);
	test_assert_us_eq("one point eq", u2, u1, 1);
	free(u1);
	free(u2);
	fclose(f);

	f = fmemopen(
	    "\x48\xc3\xa4\x6c\x6c\x6f\x2c\x20\x77\xc3\xb6\x72\x6c\x64\x21", 15,
	    "r");
	u1	   = utf8_read_file(f, &c);
	u2	   = uc_from_ascii_str("Hello, world!");
	u2[1].code = 0xe4;
	u2[8].code = 0xf6;
	test_assert("complex nonnull", u1);
	test_assert("complex count", c == 13);
	test_assert_us_eq("one point eq", u2, u1, 13);
	free(u1);
	free(u2);
	fclose(f);

	test_end();
}
