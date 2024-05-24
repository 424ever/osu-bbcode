#include "uc.h"
#include "unicode.h"

#ifndef COMMON_H
#define TEST_H

void test_assert_cp_eq(const char *name, uc_codepoint a, uc_codepoint b);
uc_codepoint make_cp(int err, uint32_t code);

#endif /* !TEST_H */
