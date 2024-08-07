#include "unicode.h"

#ifndef PARSE_H
#define PARSE_H
struct parser
{
	uc_string source;
	size_t	  pos;
};

struct doc_result
{
	int ok;
	struct
	{
		struct bbcode_doc *doc;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct frag_list_result
{
	int ok;
	struct
	{
		struct bbcode_frag_list *frag_list;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct frag_result
{
	int ok;
	struct
	{
		struct bbcode_frag *frag;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct tag_result
{
	int ok;
	struct
	{
		uc_string tag_name;
		uc_string addit;
		int	  open;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct cp_result
{
	int ok;
	struct
	{
		uc_codepoint cp;
	} success;
	struct
	{
		const char *message;
	} error;
};

struct tag_result	parse_tag(struct parser *);
struct frag_result	parse_frag(struct parser *);
struct frag_list_result parse_fragments(struct parser *);
void			parser_init(struct parser *, uc_string);
struct doc_result	parse_doc(uc_string);
struct bbcode_doc      *bbcode_parse(FILE *);
#endif
