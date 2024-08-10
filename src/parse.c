#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "bbcode.h"
#include "parse.h"
#include "unicode.h"
#include "utf8.h"

#define TODO()                                                        \
	do                                                            \
	{                                                             \
		fprintf(stderr, "TODO: %s:%d\n", __FILE__, __LINE__); \
		abort();                                              \
	} while (0);

#ifndef TEST
static void parser_error(struct parser *p, const char *callsite,
			 const char *message)
{
	fprintf(stderr,
		"error: '%s'\n"
		"in:    '%s'\n"
		"offset: %zd\n",
		message, callsite, p->pos);
}
#else
static void parser_error(struct parser *p, const char *c, const char *m)
{
	(void) p;
	(void) c;
	(void) m;
}
#endif

static int parser_eof(struct parser *p)
{
	return uc_strlen(p->source) == p->pos;
}

static int parser_consume(struct parser *p, const char *callsite,
			  uc_codepoint *c)
{
	if (parser_eof(p))
	{
		parser_error(p, callsite, "unexpected end-of-file");
		return 0;
	}
	else
	{
		if (c)
			*c = uc_string_get(p->source, p->pos);
		++p->pos;
		return 1;
	}
}

static int parser_peek(struct parser *p, const char *callsite, uc_codepoint *c)
{
	int res;

	res = parser_consume(p, callsite, c);

	if (res)
		--p->pos;

	return res;
}

int parse_tag_attrs(struct parser *p, uc_string *tag_name, uc_string *param,
		    int *open)
{
	uc_codepoint c;
	size_t	     namestart;
	size_t	     namelen;
	size_t	     paramstart;
	size_t	     paramlen;
	int	     isopen;

	if (!parser_consume(p, __func__, &c))
		return 0;

	if (!uc_eq(c, uc_from_ascii('[')))
	{
		parser_error(
		    p, __func__,
		    "internal: tried to read tag attributes, but didn't "
		    "start at '['");
		return 0;
	}

	if (!parser_peek(p, __func__, &c))
		return 0;

	if (uc_eq(c, uc_from_ascii('/')))
	{
		isopen	  = 0;
		namestart = p->pos + 1;
		(void) parser_consume(p, __func__, NULL);
	}
	else
	{
		isopen	  = 1;
		namestart = p->pos;
	}

	namelen	   = 0;
	paramstart = 0;
	for (;;)
	{
		if (!parser_consume(p, __func__, &c))
			return 0;

		if (uc_eq(c, uc_from_ascii(' ')))
		{
			parser_error(p, __func__,
				     "tag name cannot contain ' '");
			return 0;
		}
		if (uc_eq(c, uc_from_ascii('[')))
		{
			parser_error(p, __func__,
				     "tag name cannot contain '['");
			return 0;
		}
		if (uc_eq(c, uc_from_ascii('=')))
		{
			paramstart = p->pos;
			break;
		}
		else if (uc_eq(c, uc_from_ascii(']')))
			break;
		++namelen;
	}

	/*
	 * We can check if paramstart isn't 0 here, because this function checks
	 * that the first character is `[`, and therefore not `=`.
	 */
	paramlen = 0;
	if (paramstart)
	{
		for (;;)
		{
			if (!parser_consume(p, __func__, &c))
				return 0;
			if (uc_eq(c, uc_from_ascii('[')))
			{
				parser_error(
				    p, __func__,
				    "tag parameter cannot contain '['");
				return 0;
			}
			if (uc_eq(c, uc_from_ascii(']')))
				break;
			++paramlen;
		}
	}

	if (paramstart && !isopen)
	{
		parser_error(p, __func__, "closing tag cannot have '='");
		return 0;
	}

	if (tag_name)
		*tag_name = uc_string_view(p->source, namestart, namelen);
	if (param)
		*param = uc_string_view(p->source, paramstart, paramlen);
	if (open)
		*open = isopen;
	return 1;
}

void parser_init(struct parser *p, uc_string source)
{
	p->source = source;
	p->pos	  = 0;
}

struct bbcode_doc *bbcode_parse(FILE *ifile)
{
	uc_string source;

	source = utf8_read_file(ifile);
	if (source == NULL)
	{
		fprintf(stderr, "error reading input: %s\n", uc_last_error());
		return NULL;
	}

	TODO();

	return NULL;
}
