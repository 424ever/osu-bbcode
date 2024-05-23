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
uc_codepoint uc_from_ascii(const char c);

/*
 * Finds if a unicode codepoint is in the range [0..127].
 */
int uc_is_ascii(uc_codepoint c);

/*
 * Converts a NUL-terminated ASCII string to a string of unicode
 * codepoints. The resulting string needs to be free'd by the caller.
 */
uc_codepoint *uc_from_ascii_str(const char *str);

/*
 * Converts a string of unicode codepoints to a NUL-terminated ASCII
 * ASCII string. All non-ASCII characters are replaced with '_'. The
 * resulting string needs to be free'd by the caller.
 */
char *uc_to_ascii_str(const uc_codepoint *str);

/*
 * Gets the length of a NUL-terminated string of unicode codepoints.
 */
size_t uc_strlen(const uc_codepoint *str);
#endif /* !UNICODE_H */
