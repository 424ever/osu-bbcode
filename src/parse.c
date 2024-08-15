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

static int parse_tag(struct parser *p, struct bbcode_frag **f);

static void parser_error(struct parser *p, const char *message)
{
	if (p->report_errors)
		report_error("error: '%s' "
			     "offset: %zd",
			     message, p->pos);
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

	ret = parse_tag_attrs_int(p, tag_name, param, open);

	return ret;
}

static int peek_tag_attrs(struct parser *p, uc_string *name, uc_string *param,
			  int *open)
{
	size_t oldpos;
	int    ret;

	oldpos = p->pos;

	p->report_errors = 0;

	ret = parse_tag_attrs_int(p, name, param, open);

	p->pos		 = oldpos;
	p->report_errors = 1;

	return ret;
}

int parse_text(struct parser *p, uc_string *text)
{
	uc_string    s;
	uc_codepoint c;
	int	     ret;

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
			if (peek_tag_attrs(p, NULL, NULL, NULL))
				break;
			++p->pos;
		}
		uc_string_append(s, c);
	}

	if (text && ret)
		*text = s;

	return ret;
}

static int parse_frag_list_int(struct parser *p, struct bbcode_frag_list **l,
			       uc_string until_end_tag)
{
	uc_string		 tag_name;
	int			 open;
	struct bbcode_frag	*f;
	struct bbcode_frag_list *ll;
	uc_string		 text;

	ll = frag_list_new();

	for (;;)
	{
		if (parser_eof(p))
		{
			if (!until_end_tag)
				break;
			else
			{
				parser_error(
				    p, "improper nesting: expected to read "
				       "until a closing tag, but found EOF");
				goto error;
			}
		}

		if (peek_tag_attrs(p, &tag_name, NULL, &open))
		{
			if (until_end_tag && !open)
			{
				if (uc_strcmp(until_end_tag, tag_name))
				{
					parser_error(p,
						     "improper nesting: "
						     "wrong closing tag found");
					goto free_name_error;
				}
				else
				{
					(void) parse_tag_attrs(p, NULL, NULL,
							       NULL);
					uc_string_free(tag_name);
					break;
				}
			}
			else
			{
				if (!open)
				{
					parser_error(
					    p, "improper nesting: expected to "
					       "read until EOF, but found "
					       "closing tag");
					goto free_name_error;
				}
			}

			if (parse_tag(p, &f))
				frag_list_append(ll, f);
			else
				goto free_name_error;

			uc_string_free(tag_name);
		}
		else
		{
			if (parse_text(p, &text))
				frag_list_append(ll, frag_text_new(text));
			else
				return 0;
		}
	}

	if (l)
		*l = ll;
	else
		frag_list_free(ll);
	return 1;

free_name_error:
	uc_string_free(tag_name);
error:
	frag_list_free(ll);
	return 0;
}

static int parse_tag(struct parser *p, struct bbcode_frag **f)
{
	uc_string		 tag_name;
	uc_string		 param;
	int			 open;
	int			 ret;
	struct bbcode_frag_list *children;

	/* No need to check the return code, since this should only be called
	 * when a tag was successfully peek'd. */
	(void) parse_tag_attrs(p, &tag_name, &param, &open);

	if (!open)
	{
		parser_error(
		    p, "improper nesting: did not expect a closing tag here");
		goto error_strings;
	}

	ret = parse_frag_list_int(p, &children, tag_name);
	if (!ret)
		goto error_strings;

	if (f)
	{
		*f = frag_tag_new(tag_name, param);
		frag_tag_append_all(*f, children);
		frag_list_leak(children);
	}
	else
	{
		uc_string_free(tag_name);
		uc_string_free(param);
		frag_list_free(children);
	}

	return 1;

error_strings:
	uc_string_free(tag_name);
	uc_string_free(param);
	return 0;
}

int parse_frag(struct parser *p, struct bbcode_frag **f)
{
	uc_string text;

	if (peek_tag_attrs(p, NULL, NULL, NULL))
		return parse_tag(p, f);
	else
	{
		if (parse_text(p, &text))
		{
			*f = frag_text_new(text);
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int parse_frag_list(struct parser *p, struct bbcode_frag_list **l)
{
	return parse_frag_list_int(p, l, NULL);
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
