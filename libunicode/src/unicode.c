#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uc.h"
#include "unicode.h"

int uc_str_has_error_(const uc_codepoint *s)
{
	size_t i;

        i = 0;

	for (;;)
	{
		if (uc_is_err(s[i]))
			return 1;

                if(uc_is_nul(s[i]))
                        return 0;

                ++i;
	}
	return 0;
}

static int is_nul_unchecked(uc_codepoint c)
{
	return c.code == 0;
}

uc_codepoint uc_from_ascii(char c)
{
	uc_codepoint p;

	if ((c & 0x80) == 0)
	{
		p.err  = 0;
		p.code = c;
	}
	else
	{
		p.err = 1;
	}

	return p;
}

int uc_is_ascii(uc_codepoint c)
{
	return !uc_is_err(c) && c.code <= 127;
}

uc_codepoint *uc_from_ascii_str(const char *str)
{
	uc_codepoint *ustr;
	size_t	      i;
	size_t	      len;

	len = strlen(str);

        ustr = calloc(len + 1, sizeof(uc_codepoint));
	if (ustr == NULL)
		return NULL;

	for (i = 0; i < len; ++i)
		ustr[i] = uc_from_ascii(str[i]);

	ustr[len] = uc_make_nul();

	if (uc_str_has_error_(ustr))
	{
		free(ustr);
		return NULL;
	}

	return ustr;
}

char *uc_to_ascii_str(const uc_codepoint *ustr)
{
	char	    *str;
	size_t	     i;
	size_t	     len;
	uc_codepoint c;

	len = uc_strlen(ustr);

	if (uc_str_has_error_(ustr))
		return NULL;

	if ((str = calloc(len + 1, sizeof(*str))) == NULL)
		return NULL;

	for (i = 0; i < len; ++i)
	{
		c = ustr[i];

		if (uc_is_ascii(ustr[i]))
			str[i] = (char) c.code;
		else
			str[i] = '_';
	}

	return str;
}

size_t uc_strlen(const uc_codepoint *str)
{
	size_t	     len;
	uc_codepoint c;

	len = 0;

        if (uc_str_has_error_(str))
                return 0;

	for (;;)
	{
		c = str[len];

		if (uc_is_nul(c))
			break;

		++len;
	}

	return len;
}

int uc_is_err(uc_codepoint c)
{
	return !!c.err;
}

int uc_is_nul(uc_codepoint c)
{
	return !uc_is_err(c) && is_nul_unchecked(c);
}

uc_codepoint uc_make_nul(void)
{
	uc_codepoint c;
	c.err  = 0;
	c.code = 0;
	return c;
}

int uc_strcmp(const uc_codepoint *a, const uc_codepoint *b)
{
	size_t i;

	if (uc_str_has_error_(a) || uc_str_has_error_(b))
		return -69;

	for (i = 0;; ++i)
	{
		if (a[i].code < b[i].code)
			return -1;
		if (a[i].code > b[i].code)
			return 1;
		if (uc_is_nul(a[i]))
			return 0;
	}
}
