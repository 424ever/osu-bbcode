#include <stdio.h>

#include "arena.h"

#ifndef PARSE_H
#define PARSE_H

struct bbcode_doc;
struct bbcode_frag;

struct bbcode_doc *bbcode_parse(FILE *ifile, struct alloc_arena *);
#endif /* !PARSE_H */
