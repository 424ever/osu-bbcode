#include "bbcode.h"
#include "alloc.h"

void frag_list_append(struct bbcode_frag_list *l, struct bbcode_frag *f)
{
	while (l->next != NULL)
		l = l->next;
	l->next = safe_alloc("frag_list_append", 1, sizeof(*l->next));
	l	= l->next;
	l->frag = f;
	l->next = NULL;
}
