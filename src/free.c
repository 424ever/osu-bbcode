#include <stdlib.h>

#include "bbcode.h"

static void frag_free(struct bbcode_frag *);
static void frag_list_free(struct bbcode_frag_list *);

static void frag_free(struct bbcode_frag *f)
{
	switch (f->type)
	{
	case TAG:
		free((void *) f->tag.name);
		frag_list_free((void *) f->tag.children);
		break;
	case TEXT:
		free((void *) f->text.text);
		break;
	}
	free(f);
}

static void frag_list_free(struct bbcode_frag_list *l)
{
	struct bbcode_frag_list *cur;
	struct bbcode_frag_list *prev;

	cur = l;
	while (cur != NULL)
	{
		prev = cur;
		cur  = cur->next;

		frag_free(prev->frag);
		free((void *) prev);
	}
}

void bbcode_doc_free(struct bbcode_doc *doc)
{
	frag_list_free(doc->root_fragments);
	free((void *) doc);
}
