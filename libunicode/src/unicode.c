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

int uc_is_ascii(uc_codepoint c)
{
	return c <= 127;
}

uc_codepoint *uc_from_ascii_str(const char *str)
{
	uc_codepoint *ustr;
	size_t	      i;
	size_t	      len;

	len = strlen(str);

	if ((ustr = calloc(len + 1, sizeof(uc_codepoint))) == NULL)
		return NULL;

	for (i = 0; i < len; ++i)
		ustr[i] = uc_from_ascii(str[i]);

	return ustr;
}

char *uc_to_ascii_str(const uc_codepoint *ustr)
{
	char  *str;
	size_t i;
	size_t len;

	len = uc_strlen(ustr);

	if ((str = calloc(len + 1, sizeof(*str))) == NULL)
		return NULL;

	for (i = 0; i < len; ++i)
		if (uc_is_ascii(ustr[i]))
			str[i] = (char) ustr[i];
		else
			str[i] = '_';

	return str;
}

size_t uc_strlen(const uc_codepoint *str)
{
	size_t len;

	len = 0;

	while (str[++len] != UC_NUL)
		;

	return len;
}
