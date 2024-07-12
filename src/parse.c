#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "bbcode.h"
#include "unicode.h"

struct parser
{
	uc_string	   source;
	size_t		   pos;
	struct bbcode_doc *doc;
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

static void parser_parse_doc(struct parser *p)
{
	(void) p;
}

struct bbcode_doc *bbcode_parse(FILE *ifile)
{
	size_t	      sourcelen;
	uc_string     source;
	struct parser parser;

	sourcelen = utf8_read_file(ifile, &source);
	if (sourcelen == (size_t) -1)
	{
		fprintf(stderr, "error reading input: %s\n", uc_last_error());
		return NULL;
	}

	parser.source = source;
	parser.pos    = 0;
	parser.doc =
	    safe_alloc("bbcode_parse create document", 1, sizeof(*parser.doc));

	parser_parse_doc(&parser);
	uc_string_free(source);
	return parser.doc;
}
