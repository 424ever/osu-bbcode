#include <inttypes.h>
#include <stdio.h>

#include "unicode.h"

#ifndef PARSE_H
#define PARSE_H

struct bbcode_frag_list
{
	struct bbcode_frag	*frag;
	struct bbcode_frag_list *next;
};

struct bbcode_doc
{
	struct bbcode_frag_list *root_fragments;
};

enum bbcode_frag_type
{
	TEXT,
	TAG
};

struct bbcode_frag
{
	enum bbcode_frag_type type;
	union
	{
		struct
		{
			const uc_codepoint *text;
		} text;
		struct
		{
			const uc_codepoint	      *name;
			const struct bbcode_frag_list *children;
		} tag;
	};
};

struct bbcode_doc *bbcode_parse(FILE *ifile);
void		   bbcode_doc_free(struct bbcode_doc *doc);
#endif /* !PARSE_H */
