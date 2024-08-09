#include "unicode.h"

#ifndef BBCODE_H
#define BBCODE_H

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

void bbcode_doc_free(struct bbcode_doc *doc);
void frag_list_append(struct bbcode_frag_list *l, struct bbcode_frag *f);
#endif /* !BBCODE_H */
