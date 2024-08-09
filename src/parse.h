#include "unicode.h"

#ifndef PARSE_H
#define PARSE_H
struct parser
{
	uc_string source;
	size_t	  pos;
};

void parser_init(struct parser *, uc_string);
int  parse_tag_attrs(struct parser *, uc_string *tag_name, uc_string *param,
		     int *open);
struct bbcode_doc *bbcode_parse(FILE *);
#endif
