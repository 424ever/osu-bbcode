#include <stdio.h>

#include "bbcode.h"

struct bbcode_doc *bbcode_parse(FILE *ifile, struct alloc_arena *a)
{
	(void) ifile;
	return (struct bbcode_doc *) arena_alloc(a, sizeof(struct bbcode_doc));
}
