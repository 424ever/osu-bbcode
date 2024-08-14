#include <stdint.h>
#include <stdio.h>

#ifndef UNICODE_H
#define UNICODE_H

#define RETURN_WITH_ERROR_SET(cp) \
	{                         \
		cp.err	= 1;      \
		cp.code = 0;      \
		return cp;        \
	}

struct uc_codepoint_
{
	int	 err;
	uint32_t code;
};

struct uc_string_;

typedef struct uc_codepoint_ uc_codepoint;
typedef struct uc_string_   *uc_string;

int uc_str_has_error_(const uc_string);

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
 * Compares 2 codepoints for equality. If either codepoints has it's error flag
 * set (even both), they are not considered equal.
 */
int uc_eq(uc_codepoint, uc_codepoint);

/*
 * Checks if the given codepoint has it's error flag set.
 */
int uc_is_err(uc_codepoint);

/*
 * Converts a NUL-terminated ASCII string to a string of unicode
 * codepoints. If any of the characters are out of range, the corresponding
 * codepoints will have their error flag set.
 */
uc_string uc_from_ascii_str(const char *);

/*
 * Converts a string of unicode codepoints to a NUL-terminated ASCII
 * ASCII string. All non-ASCII characters are replaced with '_'. if any of the
 * codepoints have their error flag set, `NULL` is returned.
 */
char *uc_to_ascii_str(const uc_string);

/*
 * Gets the length of a NUL-terminated string of unicode codepoints. If
 * any of the codepoints have their error flag set, 0 is returned.
 */
size_t uc_strlen(const uc_string);

/*
 * Compares two string of unicode codepoints the same way `strcmp()`
 * does with strings of `char`s, but will always return either -1, 0,
 * or 1 if successfull and -69 if any of the string has a codepoint
 * with a set error flag.
 */
int uc_strcmp(const uc_string, const uc_string);

/*
 * Create a new string with the specified length. All characters will be NUL.
 */
uc_string uc_string_new(size_t len);

/*
 * Create a new string with a specified length, copying that many codepoints
 * from a buffer.
 */
uc_string uc_string_from_buf(uc_codepoint *, size_t);

/*
 * Exposes a subsection of a string in the form of another string. Calling
 * `uc_string_free()` with the resulting string is a no-op, because it makes no
 * allocation. After `uc_string_free()` has been called on the string which owns
 * the allocation (one created via `uc_string_new()` for example), any
 * operations on a string returned by this function is undefined behaviour. If
 * an attempt is made to create a view which exceeds the bounds of the original
 * string, the program is abort()ed.
 */
uc_string uc_string_view(uc_string, size_t start, size_t len);

/*
 * Frees a unicode string. No more operations are allowed on the string
 * afterwards.
 */
void uc_string_free(uc_string);

/*
 * Get the codepoint at the specified index from a string of unicode codepoints.
 * If the index is out of range, the program will be abort()ed.
 */
uc_codepoint uc_string_get(uc_string, size_t i);

/*
 * Set the codepoint at the specified index in a unicode string. If the index is
 * out of range, the program will be abort()ed.
 */
void uc_string_set(uc_string, size_t i, uc_codepoint);

/*
 * Appends a codepoint to a unicode string. This is only possible if the passed
 * string owns it's allocation, i.e. was not created with `uc_string_view()`. In
 * such a case, the program is abort()ed.
 */
void uc_string_append(uc_string, uc_codepoint);

#endif /* !UNICODE_H */
