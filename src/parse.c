#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "bbcode.h"
#include "error.h"
#include "parse.h"
#include "unicode.h"
#include "utf8.h"

#define TODO()                                                        \
	do                                                            \
	{                                                             \
		fprintf(stderr, "TODO: %s:%d\n", __FILE__, __LINE__); \
		abort();                                              \
	} while (0);

static void parser_error(struct parser *p, const char *message)
{
	if (p->report_errors)
		report_error("error: '%s' "
			     "in:    '%s' "
			     "offset: %zd",
			     message, p->curfn, p->pos);
}

static int parser_eof(struct parser *p)
{
	return uc_strlen(p->source) == p->pos;
}

static int parser_consume(struct parser *p, uc_codepoint *c)
{
	if (parser_eof(p))
	{
		parser_error(p, "unexpected end-of-file");
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

static int parser_peek(struct parser *p, uc_codepoint *c)
{
	int res;

	res = parser_consume(p, c);

	if (res)
		--p->pos;

	return res;
}

void parser_init(struct parser *p, uc_string source)
{
	p->source	 = source;
	p->pos		 = 0;
	p->report_errors = 1;
	p->curfn	 = NULL;
}

static void parser_enter(struct parser *p, const char *fn)
{
	if (p->curfn)
		fprintf(stderr,
			"BUG: parser_enter called before function was "
			"exited with parser_leave. old: '%s' new: '%s'\n",
			p->curfn, fn);
	p->curfn = fn;
}

static void parser_leave(struct parser *p)
{
	p->curfn = NULL;
}

static int parse_tag_attrs_int(struct parser *p, uc_string *tag_name,
			       uc_string *param, int *open)
{
	uc_codepoint c;
	size_t	     namestart;
	size_t	     namelen;
	size_t	     paramstart;
	size_t	     paramlen;
	int	     isopen;

	if (!parser_consume(p, &c))
		return 0;

	if (!uc_eq(c, uc_from_ascii('[')))
	{
		parser_error(
		    p, "internal: tried to read tag attributes, but didn't "
		       "start at '['");
		return 0;
	}

	if (!parser_peek(p, &c))
		return 0;

	if (uc_eq(c, uc_from_ascii('/')))
	{
		isopen	  = 0;
		namestart = p->pos + 1;
		(void) parser_consume(p, NULL);
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
		if (!parser_consume(p, &c))
			return 0;

		if (uc_eq(c, uc_from_ascii(' ')))
		{
			parser_error(p, "tag name cannot contain ' '");
			return 0;
		}
		if (uc_eq(c, uc_from_ascii('[')))
		{
			parser_error(p, "tag name cannot contain '['");
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
			if (!parser_consume(p, &c))
				return 0;
			if (uc_eq(c, uc_from_ascii('[')))
			{
				parser_error(
				    p, "tag parameter cannot contain '['");
				return 0;
			}
			if (uc_eq(c, uc_from_ascii(']')))
				break;
			++paramlen;
		}
	}

	if (paramstart && !isopen)
	{
		parser_error(p, "closing tag cannot have '='");
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

int parse_tag_attrs(struct parser *p, uc_string *tag_name, uc_string *param,
		    int *open)
{
	int ret;

	parser_enter(p, "parse_tag_attrs");

	ret = parse_tag_attrs_int(p, tag_name, param, open);

	parser_leave(p);

	return ret;
}

static int is_at_tag_attr(struct parser *p)
{
	size_t oldpos;
	int    ret;

	oldpos = p->pos;

	p->report_errors = 0;

	ret = parse_tag_attrs_int(p, NULL, NULL, NULL);

	p->pos		 = oldpos;
	p->report_errors = 1;

	return ret;
}

int parse_text(struct parser *p, uc_string *text)
{
	uc_string    s;
	uc_codepoint c;
	int	     ret;

	parser_enter(p, "parse_text");

	s   = uc_string_new(0);
	ret = 1;

	for (;;)
	{
		if (parser_eof(p))
			break;
		parser_consume(p, &c);

		if (uc_eq(c, uc_from_ascii('[')))
		{
			/*
			 * This is safe, because we already read at least the
			 * `[`.
			 */
			--p->pos;
			if (is_at_tag_attr(p))
				break;
			++p->pos;
		}
		uc_string_append(s, c);
	}

	if (text && ret)
		*text = s;

	parser_leave(p);

	return ret;
}

struct bbcode_doc *bbcode_parse(FILE *ifile)
{
	uc_string source;

	unset_error();
	source = utf8_read_file(ifile);
	if (source == NULL)
	{
		fprintf(stderr, "error reading input: %s\n", get_error());
		return NULL;
	}

	TODO();

	return NULL;
}
