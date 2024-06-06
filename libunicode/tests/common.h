#include "uc.h"
#include "unicode.h"

#ifndef COMMON_H
#define COMMON_H

void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b);
void test_assert_cp_ok(const char *name, uc_codepoint);
void test_assert_cp_err(const char *name, uc_codepoint);
void test_assert_us_eq(const char *name, const uc_codepoint *,
		       const uc_codepoint *, size_t);

uc_codepoint make_cp(int err, uint32_t code);

#endif /* !TEST_H */
