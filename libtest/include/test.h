#ifndef TEST_H
#define TEST_H

void test_assert(const char *name, int cond);
void test_end(void);
void test_fail(const char *name);
void test_pass(void);

#endif /* !TEST_H */
