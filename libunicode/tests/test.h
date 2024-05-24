#include "unicode.h"

#ifndef TEST_H
#define TEST_H

void test_assert(const char *name, int cond);
void test_end(void);

uc_codepoint make_cp(int err, uint32_t code);

#endif /* !TEST_H */
