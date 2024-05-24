#include "unicode.h"

#ifndef TEST_H
#define TEST_H

void test_assert(const char *name, int cond);
void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b);
void test_end(void);

uc_codepoint make_cp(int err, uint32_t code);

#endif /* !TEST_H */
