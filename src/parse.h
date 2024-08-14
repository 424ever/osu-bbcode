#include "unicode.h"

#ifndef PARSE_H
#define PARSE_H
struct parser
{
	uc_string   source;
	size_t	    pos;
	int	    report_errors;
	const char *curfn;
};

void parser_init(struct parser *, uc_string);
int  parse_tag_attrs(struct parser *, uc_string *tag_name, uc_string *param,
		     int *open);
int  parse_text(struct parser *, uc_string *text);
struct bbcode_doc *bbcode_parse(FILE *);
#endif
