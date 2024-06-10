#include <stdio.h>

#include "arena.h"

#ifndef PARSE_H
#define PARSE_H

struct bbcode_doc
{
	size_t source_size;
	char  *source;
};

struct bbcode_doc *bbcode_parse(FILE *ifile, struct alloc_arena *);
#endif /* !PARSE_H */
