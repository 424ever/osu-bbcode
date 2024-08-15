#include "unicode.h"

#ifndef BBCODE_H
#define BBCODE_H

struct bbcode_frag;
struct bbcode_frag_list;

struct bbcode_doc
{
	struct bbcode_frag_list *root_fragments;
};

enum bbcode_frag_type
{
	TEXT,
	TAG
};

struct bbcode_doc	*bbcode_doc_new(void);
void			 doc_append(struct bbcode_doc *, struct bbcode_frag *);
void			 bbcode_doc_free(struct bbcode_doc *doc);
uc_string		 doc_debug(struct bbcode_doc *);
struct bbcode_frag	*frag_text_new(uc_string);
struct bbcode_frag	*frag_tag_new(uc_string tag_name, uc_string param);
void			 frag_free(struct bbcode_frag *);
struct bbcode_frag_list *frag_list_new(void);
void frag_list_append(struct bbcode_frag_list *l, struct bbcode_frag *f);
void frag_tag_append(struct bbcode_frag *tag, struct bbcode_frag *f);
void frag_tag_append_all(struct bbcode_frag *tag, struct bbcode_frag_list *l);
void frag_list_free(struct bbcode_frag_list *);
/*
 * free the list and it's buffer, but not the fragments inside the list.
 */
void	  frag_list_leak(struct bbcode_frag_list *);
uc_string frag_debug(struct bbcode_frag *);
uc_string frag_list_debug(struct bbcode_frag_list *);
#endif /* !BBCODE_H */
