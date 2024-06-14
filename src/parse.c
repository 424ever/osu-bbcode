#include <stdio.h>

#include "bbcode.h"
#include "unicode.h"

struct frag_list
{
	const struct bbcode_frag *frag;
	struct frag_list	 *next;
};

struct bbcode_doc
{
	struct frag_list *root_fragments;
};

enum frag_type
{
	TEXT,
	TAG
};

struct bbcode_frag
{
	enum frag_type type;
	union
	{
		struct
		{
			const uint64_t start;
			const uint64_t len;
		} text;
		struct
		{
			const uc_codepoint     *name;
			const struct frag_list *children;
		} tag;
	};
};

struct parser
{
	uc_codepoint	  *source;
	size_t		   sourcelen;
	size_t		   pos;
	struct bbcode_doc *doc;
};

static void frag_list_append(struct frag_list *l, const struct bbcode_frag *f,
			     struct alloc_arena *a)
{
	while (l->next != NULL)
		l = l->next;
	l->next = arena_alloc(a, sizeof(*l->next));
	l	= l->next;
	l->frag = f;
	l->next = NULL;
}

static struct parser *parser_create(uc_codepoint *source, size_t sourcelen,
				    struct alloc_arena *a)
{
	struct parser *p = arena_alloc(a, sizeof(*p));

	p->source    = source;
	p->sourcelen = sourcelen;
	p->pos	     = 0;
	p->doc	     = arena_alloc(a, sizeof(*p->doc));

	return p;
}

static void parser_parse_doc(struct parser *p)
{
	(void) p;
}

struct bbcode_doc *bbcode_parse(FILE *ifile, struct alloc_arena *a)
{
	size_t	      sourcelen;
	uc_codepoint *source = utf8_read_file(ifile, &sourcelen, a);
	if (sourcelen == (size_t) -1)
	{
		return NULL;
	}

	struct parser *p = parser_create(source, sourcelen, a);
	parser_parse_doc(p);
	return p->doc;
}
