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
 * Reads an entire UTF-8 encoded file into a new string of codepoints.
 * The file is read up to the first errornous codepoint, which is also written
 * to the buffer. On success, the function returns the new string read, on
 * failure NULL. In this case, success includes encountering an erronous
 * codepoint.
 * This function reports an error if an invalid codepoint is
 * encountered, or an IO error occurs.
 */
uc_string utf8_read_file(FILE *);
#endif
