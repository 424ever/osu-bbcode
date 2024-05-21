#include <stdio.h>

#ifndef PARSE_H
struct bbcode_doc
{
	size_t source_size;
	char  *source;
};

struct bbcode_doc *bbcode_parse(FILE *ifile);
void		   bbcode_free(struct bbcode_doc *doc);
#endif /* !PARSE_H */
