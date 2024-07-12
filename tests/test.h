#include "unicode.h"

#ifndef TEST_H
#define TEST_H

void test_assert(const char *name, int cond);
void test_assert_eq_i(const char *name, int a, int b);
void test_end(void);
void test_fail(const char *name);
void test_pass(const char *name);
void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b);
void test_assert_cp_ok(const char *name, uc_codepoint);
void test_assert_cp_err(const char *name, uc_codepoint);
void test_assert_us_eq(const char *name, const uc_string, const uc_string);
void test_assert_no_error(const char *name);
void test_assert_error(const char *name);

uc_codepoint make_cp(int err, uint32_t code);

#endif /* !TEST_H */
