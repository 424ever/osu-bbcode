#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unicode.h"

uc_codepoint uc_from_ascii(char c)
{
	if ((c & 0x80) == 0)
		return (uc_codepoint) c;

	fprintf(stderr, "Attempted to convert ASCII %x to unicode",
		(unsigned int) c);
	exit(1);
}

uc_codepoint *uc_from_ascii_str(char *str)
{
	uc_codepoint *ustr;
	size_t	      i;
	size_t	      len;

	len = strlen(str);

	if ((ustr = calloc(len, sizeof(uc_codepoint))) == NULL)
		return NULL;

	for (i = 0; i < len; ++i)
		ustr[i] = uc_from_ascii(str[i]);

	return ustr;
}

size_t uc_strlen(uc_codepoint *str)
{
	size_t len;

	len = 0;

	while (str[++len] != UC_NUL)
		;

	return len;
}
