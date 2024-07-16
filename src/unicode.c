#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "unicode.h"

int uc_str_has_error_(const uc_string s)
{
	size_t i;

	for (i = 0; i < s.len; ++i)
	{
		if (uc_is_err(uc_string_get(s, i)))
			return 1;
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

	if ((c & 0x80) != 0)
	{
		uc_set_error_("uc_from_ascii: 0x%x is out of range.", c);
		RETURN_WITH_ERROR_SET(p);
	}

	p.err  = 0;
	p.code = c;
	return p;
}

int uc_is_ascii(uc_codepoint c)
{
	return !uc_is_err(c) && c.code <= 127;
}

int uc_eq(uc_codepoint a, uc_codepoint b)
{
	return !uc_is_err(a) && !uc_is_err(b) && a.code == b.code;
}

uc_string uc_from_ascii_str(const char *str)
{
	uc_string ustr;
	size_t	  i;
	size_t	  len;

	uc_unset_error_();
	len = strlen(str);

	ustr = uc_string_new(len);

	for (i = 0; i < len; ++i)
		uc_string_set(ustr, i, uc_from_ascii(str[i]));

	return ustr;
}

char *uc_to_ascii_str(const uc_string ustr)
{
	char	    *str;
	size_t	     i;
	size_t	     len;
	uc_codepoint c;

	len = uc_strlen(ustr);

	if (uc_str_has_error_(ustr))
		return NULL;

	str = safe_alloc("uc_to_ascii_str", len + 1, sizeof(*str));
	memset(str, '\0', len + 1);

	for (i = 0; i < len; ++i)
	{
		c = uc_string_get(ustr, i);

		if (uc_is_ascii(c))
			str[i] = (char) c.code;
		else
			str[i] = '_';
	}

	return str;
}

size_t uc_strlen(const uc_string str)
{
	return str.len;
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

int uc_strcmp(const uc_string a, const uc_string b)
{
	size_t i;
	size_t min;
	size_t lena;
	size_t lenb;

	if (uc_str_has_error_(a) || uc_str_has_error_(b))
		return -69;

	lena = uc_strlen(a);
	lenb = uc_strlen(b);
	min  = lena < lenb ? lena : lenb;

	for (i = 0; i < min; ++i)
	{
		if (uc_string_get(a, i).code < uc_string_get(b, i).code)
			return -1;
		if (uc_string_get(a, i).code > uc_string_get(b, i).code)
			return 1;
	}

	// When the common part is the same, the shorter string is smaller
	if (lena == lenb)
		return 0;
	else if (lena < lenb)
		return -1;
	else
		return 1;
}

uc_string uc_string_new(size_t len)
{
	uc_string s;

	s.len	= len;
	s.buf	= safe_alloc("uc_string_new", len, sizeof(*s.buf));
	s.owner = 1;

	return s;
}

uc_string uc_string_from_buf(uc_codepoint *buf, size_t len)
{
	uc_string s;
	size_t	  i;

	s = uc_string_new(len);
	for (i = 0; i < len; ++i)
	{
		uc_string_set(s, i, buf[i]);
	}

	return s;
}

uc_string uc_string_view(uc_string str, size_t start, size_t len)
{
	uc_string view;

	if (len > (str.len - start))
	{
		fprintf(stderr,
			"attempted to create a view of %zd codepoints starting "
			"from %zd, for a string with %zd, codepoints",
			len, start, str.len);
		abort();
	}

	view.buf   = str.buf + start;
	view.len   = len;
	view.owner = 0;

	return view;
}

void uc_string_free(uc_string s)
{
	if (s.owner)
		free(s.buf);
}

void uc_string_set(uc_string s, size_t i, uc_codepoint c)
{
	if (i >= s.len)
	{
		fprintf(stderr,
			"attempted to set char %zd in string of length %zd.\n",
			i, s.len);
		abort();
	}
	s.buf[i] = c;
}

uc_codepoint uc_string_get(uc_string s, size_t i)
{
	if (i >= s.len)
	{
		fprintf(stderr,
			"attempted to get char %zd in string of length %zd.\n",
			i, s.len);
		abort();
	}
	return s.buf[i];
}
