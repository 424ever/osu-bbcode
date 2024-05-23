#include <stdint.h>

#ifndef UNICODE_H
typedef uint32_t uc_codepoint;

/*
 * Convers an ascii code in the range [0..127] to a unicode
 * codepoint. If the value is out of range, an error message is
 * printed, and exits with an error code.
 */
uc_codepoint from_ascii(uint8_t c);
#endif /* !UNICODE_H */
