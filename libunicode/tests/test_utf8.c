#define _GNU_SOURCE
#include <stdio.h>

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
	FILE *f;

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

	test_end();
}
