#include <errno.h>
#include <error.h>
#include <stdint.h>

#include "unicode.h"

uc_codepoint from_ascii(uint8_t c)
{
	if (c <= 127)
		return (uc_codepoint) c;

	error(1, ENOTSUP, "Attempted to convert ASCII %x to unicode",
	      (unsigned int) c);
	return -1;
}
