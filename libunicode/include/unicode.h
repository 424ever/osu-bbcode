#include <stdint.h>
#include <stdlib.h>

#ifndef UNICODE_H
#define UC_NUL 0

typedef uint32_t uc_codepoint;

/*
 * Converts an ascii code in the range [0..127] to a unicode
 * codepoint. If the value is out of range, an error message is
 * printed, and exits with an error code.
 */
uc_codepoint uc_from_ascii(char c);

/*
 * Converts a NUL-terminated ASCII string to a string of unicode
 * codepoints. The resulting string needs to be free'd by the caller.
 */
uc_codepoint *uc_from_ascii_str(char *str);

/*
 * Gets the length of a NUL-terminated string of unicode codepoints.
 */
size_t uc_strlen(uc_codepoint *str);
#endif /* !UNICODE_H */
