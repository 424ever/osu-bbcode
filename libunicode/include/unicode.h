#include <stdint.h>
#include <stdio.h>

#ifndef UNICODE_H
#define UNICODE_H

typedef struct uc_codepoint_ uc_codepoint;

/*
 * Converts an ascii code in the range [0..127] to a unicode
 * codepoint. If the value is out of range, the error flag on the
 * is set.
 * If the value is out of range, this function reports an error with
 * that value.
 */
uc_codepoint uc_from_ascii(const char);

/*
 * Finds if a unicode codepoint is in the range [0..127]. If the
 * codepoint has it's error flag set, the answer is always 'no'.
 */
int uc_is_ascii(uc_codepoint);

/*
 * Checks if the given codepoint has it's error flag set.
 */
int uc_is_err(uc_codepoint);

/*
 * Checks if the given codepoint is the null character. Always false if
 * the error flag is set.
 */
int uc_is_nul(uc_codepoint);

/*
 * Makes a terminating codepoint
 */
uc_codepoint uc_make_nul(void);

/*
 * Converts a NUL-terminated ASCII string to a string of unicode
 * codepoints. The resulting string needs to be free'd by the caller.
 * If any of the characters are out of range, `NULL` is returned.
 */
uc_codepoint *uc_from_ascii_str(const char *);

/*
 * Converts a string of unicode codepoints to a NUL-terminated ASCII
 * ASCII string. All non-ASCII characters are replaced with '_'. The
 * resulting string needs to be free'd by the caller. if any of the
 * codepoints have their error flag set, `NULL` is returned.
 */
char *uc_to_ascii_str(const uc_codepoint *);

/*
 * Gets the length of a NUL-terminated string of unicode codepoints. If
 * any of the codepoints have their error flag set, 0 is returned.
 */
size_t uc_strlen(const uc_codepoint *);

/*
 * Compares two string of unicode codepoints the same way `strcmp()`
 * does with strings of `char`s, but will always return either -1, 0,
 * or 1 if successfull and -69 if any of the string has a codepoint
 * with a set error flag.
 */
int uc_strcmp(const uc_codepoint *, const uc_codepoint *);

/*
 * Compares the first `n` codepoints of two buffers the same way
 * `memcmp()` does, but will always return either -1, 0, or 1 if
 * successfull and -69 if any codepoint with it's error flag is
 * encountered.
 */
int uc_memcmp(const uc_codepoint *, const uc_codepoint *, size_t n);

/*
 * Gets the message of the last reported error. If no error was
 * reported, an empty string is returned. Only function which can
 * report an error will reset the message on success.
 * NOTE: The message uses a statically allocated buffer.
 */
const char *uc_last_error(void);

/*
 * Reads a single codepoint from an UTF-8 encoded file. The file must
 * be opened for reading. If no valid codepoint can be read, the error
 * flag is set.
 * This function reports an error if no valid codepoint can be read.
 */
uc_codepoint utf8_read_codepoint(FILE *);
#endif /* !UNICODE_H */
