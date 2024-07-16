#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "bbcode.h"
#include "unicode.h"

#define CHECK_RESULT_OK_AND_PROPAGATE_ERROR_MESSAGE(oldr, newr)  \
	do                                                       \
	{                                                        \
		if (!oldr.ok)                                    \
		{                                                \
			newr.ok		   = 0;                  \
			newr.error.message = oldr.error.message; \
			return newr;                             \
		}                                                \
	} while (0);

struct parser
{
	uc_string source;
	size_t	  pos;
};

struct doc_result
{
	int ok;
	struct
	{
		struct bbcode_doc *doc;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct frag_list_result
{
	int ok;
	struct
	{
		struct bbcode_frag_list *frag_list;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct frag_result
{
	int ok;
	struct
	{
		struct bbcode_frag *frag;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct tag_result
{
	int ok;
	struct
	{
		uc_string tag_name;
		int	  open;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct cp_result
{
	int ok;
	struct
	{
		uc_codepoint cp;
	} success;
	struct
	{
		const char *message;
	} error;
};

static void frag_list_append(struct bbcode_frag_list *l, struct bbcode_frag *f)
{
	while (l->next != NULL)
		l = l->next;
	l->next = safe_alloc("frag_list_append", 1, sizeof(*l->next));
	l	= l->next;
	l->frag = f;
	l->next = NULL;
}

static int parser_eof(struct parser *p)
{
	return uc_strlen(p->source) == p->pos;
}

static struct cp_result parser_consume(struct parser *p)
{
	struct cp_result cp;

	if (parser_eof(p))
	{
		cp.ok		 = 0;
		cp.error.message = "Unexpected end-of-file.";
	}
	else
	{
		cp.ok	      = 1;
		cp.success.cp = uc_string_get(p->source, p->pos);
		++p->pos;
	}

	return cp;
}

static struct tag_result parse_tag(struct parser *p)
{
	struct tag_result result;
	struct cp_result  cp;
	size_t		  namestart;
	size_t		  namelen;

	cp = parser_consume(p);
	CHECK_RESULT_OK_AND_PROPAGATE_ERROR_MESSAGE(cp, result);
	if (!uc_eq(cp.success.cp, uc_from_ascii('[')))
	{
		result.ok = 0;
		result.error.message =
		    "internal: tried to read tag name, but didn't start at '['";
		return result;
	}

	cp = parser_consume(p);
	CHECK_RESULT_OK_AND_PROPAGATE_ERROR_MESSAGE(cp, result);
	if (uc_eq(cp.success.cp, uc_from_ascii('/')))
	{
		result.success.open = 0;
		namestart	    = p->pos;
	}
	else
	{
		result.success.open = 1;
		namestart	    = p->pos - 1;
	}

	namelen = 0;
	for (;;)
	{
		cp = parser_consume(p);
		CHECK_RESULT_OK_AND_PROPAGATE_ERROR_MESSAGE(cp, result);

		if (uc_eq(cp.success.cp, uc_from_ascii(']')))
		{
			break;
		}
		++namelen;
	}

	result.ok		= 1;
	result.success.tag_name = uc_string_view(p->source, namestart, namelen);
	return result;
}

static struct tag_result peek_tag(struct parser *p)
{
	struct tag_result result;
	size_t		  oldpos;

	oldpos = p->pos;
	result = parse_tag(p);
	p->pos = oldpos;

	return result;
}

static struct frag_result parse_frag(struct parser *p)
{
	struct frag_result result = {0};

	(void) p;

	return result;
}

static struct frag_list_result parse_fragments(struct parser *p)
{
	struct frag_list_result result = {0};

	(void) p;

	return result;
}

static struct doc_result parse_doc(uc_string source)
{
	struct parser		p;
	struct doc_result	result = {0};
	struct frag_list_result root_result;

	p.source = source;
	p.pos	 = 0;

	root_result = parse_fragments(&p);

	if (!root_result.ok)
	{
		result.ok	     = 0;
		result.error.message = root_result.error.message;
	}
	else
	{
		result.ok = 1;
		result.success.doc =
		    safe_alloc("parse_doc", 1, sizeof(*result.success.doc));
		result.success.doc->root_fragments =
		    root_result.success.frag_list;
	}

	return result;
}

struct bbcode_doc *bbcode_parse(FILE *ifile)
{
	size_t		  sourcelen;
	uc_string	  source;
	struct doc_result result;

	sourcelen = utf8_read_file(ifile, &source);
	if (sourcelen == (size_t) -1)
	{
		fprintf(stderr, "error reading input: %s\n", uc_last_error());
		return NULL;
	}

	result = parse_doc(source);
	uc_string_free(source);

	if (!result.ok)
	{
		fprintf(stderr, "parse error: %s\n", result.error.message);
		return NULL;
	}
	else
	{
		return result.success.doc;
	}
}
