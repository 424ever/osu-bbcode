#include "unicode.h"

#ifndef UTF8_H
#define UTF8_H
/*
 * Reads a single codepoint from an UTF-8 encoded file. The file must
 * be opened for reading. If no valid codepoint can be read, the error
 * flag is set.
 * This function reports an error if no valid codepoint can be read.
 */
uc_codepoint utf8_read_codepoint(FILE *);

/*
 * Reads an entire UTF-8 encoded file into a new buffer of codepoints.
 * The buffer needs to be freed by the caller. The file is read up
 * to the first errornous codepoint, which is also written to the
 * buffer. On success, the function returns the number of codepoints read, on
 * failure (size_t)-1. In this case, success includes encountering an erronous
 * codepoint.
 * This function reports an error if an invalid codepoint is
 * encountered, or an IO error occurs.
 */
size_t utf8_read_file(FILE *, uc_string *str);
#endif
