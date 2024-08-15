#include "unicode.h"

#ifndef T_COMMON_H
#define T_COMMON_H
uc_codepoint make_cp(uint32_t code);
void	     uis(uc_string got, uc_string expected, const char *m);
void uis_n(uc_string got, uc_string expected, const char *m, const char *name);
void no_error(const char *name);
void yes_error(const char *name);
#endif
