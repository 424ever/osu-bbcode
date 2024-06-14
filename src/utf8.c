#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unicode.h"

static unsigned int count_leading_ones(uint8_t c)
{
	uint8_t	 mask;
	unsigned count;

	mask  = 0x80;
	count = 0;

	while (c & mask)
	{
		++count;
		mask >>= 1;

		if (count >= 8)
			break;
	}

	return count;
}

static uint16_t read_or_error(FILE *f)
{
	uint8_t c;
	int	ret;

	ret = fgetc(f);

	if (ret == EOF)
	{
		if (feof(f))
			uc_set_error_("utf-8: Unexpected end-of-file");
		else if (ferror(f))
			uc_set_error_("utf-8: Error while reading: %s",
				      strerror(errno));
		else
			uc_set_error_("utf-8: read returned EOF, but neither "
				      "feof nor ferror told us anything. wtf.");
		return 0xffff;
	}
	else
	{
		c = ret & 0xff;
	}

	return c;
}

static int read_error(uint16_t val)
{
	return (val & 0xff) != val;
}

static uint8_t read_get(uint16_t val)
{
	if (read_error(val))
		abort();
	return val & 0xff;
}

static uint32_t shift_in_last_bits(uint8_t from, uint32_t to, unsigned int n)
{
	uint8_t mask;

	if (n == 0)
		return to;
	if (n > 8)
		abort();

	mask = 1;
	mask <<= n;
	--mask;

	to <<= n;
	to |= (from & mask);
	return to;
}

static uc_codepoint parse_code_point(FILE *f, uint8_t *bytecount)
{
	uc_codepoint p;
	uint8_t	     b;
	uint16_t     r;
	unsigned int afterfirst;
	unsigned int ones;
	unsigned int i;

	p.err	   = 0;
	p.code	   = 0;
	*bytecount = 0;

	r = read_or_error(f);
	if (read_error(r))
		RETURN_WITH_ERROR_SET(p);

	b = read_get(r);

	ones = count_leading_ones(b);
	if (ones > 6 || ones == 1)
	{
		uc_set_error_("utf-8: Invalid first byte 0x%x", b);
		RETURN_WITH_ERROR_SET(p);
	}

	if (ones == 0)
	{
		p.code	   = b;
		*bytecount = 1;
	}
	else
	{
		afterfirst = ones - 1;
		*bytecount = ones;
		p.code	   = shift_in_last_bits(b, p.code, (7 - ones));

		for (i = 0; i < afterfirst; ++i)
		{
			r = read_or_error(f);
			if (read_error(r))
				RETURN_WITH_ERROR_SET(p);
			b = read_get(r);

			if (count_leading_ones(b) != 1)
			{
				uc_set_error_("utf-8: Invalid byte no. %u "
					      "after header: 0x%x",
					      i, b);
				RETURN_WITH_ERROR_SET(p);
			}
			p.code = shift_in_last_bits(b, p.code, 6);
		}
	}

	return p;
}

uc_codepoint utf8_read_codepoint(FILE *f)
{
	uint8_t bytecount;

	uc_unset_error_();

	return parse_code_point(f, &bytecount);
}

uc_codepoint *utf8_read_file(FILE *f, size_t *count, struct alloc_arena *a)
{
	uc_codepoint *str;
	size_t	      maxlen;
	long	      start;
	long	      end;
	size_t	      read;
	uint8_t	      bytecount;

	uc_unset_error_();
	str    = NULL;
	*count = 0;
	read   = 0;

	if ((start = ftell(f)) < 0)
	{
		uc_set_error_("utf-8: ftell: %s", strerror(errno));
		return NULL;
	}
	if (fseek(f, 0L, SEEK_END) < 0)
	{
		uc_set_error_("utf-8: fseek: %s", strerror(errno));
		return NULL;
	}
	if ((end = ftell(f)) < 0)
	{
		uc_set_error_("utf-8: ftell: %s", strerror(errno));
		return NULL;
	}
	if (fseek(f, start, SEEK_SET) < 0)
	{
		uc_set_error_("utf-8: fseek: %s", strerror(errno));
		return NULL;
	}
	maxlen = end - start;
	str    = arena_alloc(a, maxlen * sizeof(*str));

	for (*count = 0; *count < maxlen; ++*count)
	{
		if (read >= maxlen)
			break;

		str[*count] = parse_code_point(f, &bytecount);
		read += bytecount;
		if (uc_is_err(str[*count]))
		{
			*count = (size_t) -1;
			break;
		}
	}

	return str;
}
