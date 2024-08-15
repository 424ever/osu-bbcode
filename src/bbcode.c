#include "bbcode.h"
#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>

struct frag_text
{
	uc_string text;
};

struct frag_tag
{
	uc_string		 name;
	uc_string		 param;
	struct bbcode_frag_list *children;
};

struct bbcode_frag
{
	enum bbcode_frag_type type;
	union
	{
		struct frag_text text;
		struct frag_tag	 tag;
	};
};

struct bbcode_frag_list
{
	size_t		     size;
	struct bbcode_frag **fs;
};

static uc_string frag_list_debug_with_indent(struct bbcode_frag_list *fl,
					     unsigned		      level);

struct bbcode_frag *frag_text_new(uc_string text)
{
	struct bbcode_frag *f;

	f = safe_alloc("frag_text_new", 1, sizeof(*f));

	f->type	     = TEXT;
	f->text.text = text;

	return f;
}

struct bbcode_frag *frag_tag_new(uc_string tag_name, uc_string param)
{
	struct bbcode_frag *f;

	f = safe_alloc("frag_text_new", 1, sizeof(*f));

	f->type		= TAG;
	f->tag.name	= tag_name;
	f->tag.param	= param;
	f->tag.children = frag_list_new();

	return f;
}

struct bbcode_frag_list *frag_list_new(void)
{
	struct bbcode_frag_list *fl;

	fl	 = safe_alloc("frag_list_new", 1, sizeof(*fl));
	fl->size = 0;
	fl->fs	 = NULL;

	return fl;
}

void frag_list_append(struct bbcode_frag_list *fl, struct bbcode_frag *f)
{
	fl->fs = safe_realloc("frag_list_append", fl->fs, fl->size + 1,
			      sizeof(struct bbcode_frag *));
	fl->fs[fl->size++] = f;
}

void frag_tag_append(struct bbcode_frag *tag, struct bbcode_frag *f)
{
	if (tag->type != TAG)
	{
		fprintf(
		    stderr,
		    "BUG: cannot append a child to a frag which isn't a TAG");
		abort();
	}
	frag_list_append(tag->tag.children, f);
}

void frag_tag_append_all(struct bbcode_frag *tag, struct bbcode_frag_list *l)
{
	size_t i;

	if (tag->type != TAG)
	{
		fprintf(
		    stderr,
		    "BUG: cannot append a child to a frag which isn't a TAG");
		abort();
	}
	for (i = 0; i < l->size; ++i)
		frag_list_append(tag->tag.children, l->fs[i]);
}

void frag_free(struct bbcode_frag *f)
{
	switch (f->type)
	{
	case TAG:
		uc_string_free(f->tag.name);
		uc_string_free(f->tag.param);
		frag_list_free(f->tag.children);
		break;
	case TEXT:
		uc_string_free(f->text.text);
		break;
	}
	free(f);
}

void frag_list_free(struct bbcode_frag_list *l)
{
	size_t i;

	for (i = 0; i < l->size; ++i)
	{
		frag_free(l->fs[i]);
	}

	frag_list_leak(l);
}

void frag_list_leak(struct bbcode_frag_list *l)
{
	free(l->fs);
	free(l);
}

void bbcode_doc_free(struct bbcode_doc *doc)
{
	frag_list_free(doc->root_fragments);
	free((void *) doc);
}

struct bbcode_doc *bbcode_doc_new(void)
{
	struct bbcode_doc *d;

	d		  = safe_alloc("bbcode_doc_new", 1, sizeof(*d));
	d->root_fragments = frag_list_new();

	return d;
}

void doc_append(struct bbcode_doc *d, struct bbcode_frag *f)
{
	frag_list_append(d->root_fragments, f);
}

uc_string doc_debug(struct bbcode_doc *d)
{
	uc_string s;

	s = uc_from_ascii_str("<root>");
	if (d->root_fragments->size)
	{
		uc_string_append(s, uc_from_ascii('\n'));
		uc_string_concat_tmp(
		    s, frag_list_debug_with_indent(d->root_fragments, 1));
	}

	return s;
}

static uc_string debug_indent(unsigned level)
{
	unsigned  i;
	uc_string s;

	s = uc_string_new(0);

	for (i = 0; i < level * 2; ++i)
		uc_string_append(s, uc_from_ascii(' '));

	return s;
}

static uc_string frag_text_debug(struct frag_text t, unsigned level)
{
	uc_string s;

	s = debug_indent(level);
	uc_string_concat_tmp(s, uc_from_ascii_str("[text \""));
	uc_string_concat(s, t.text);
	uc_string_concat_tmp(s, uc_from_ascii_str("\"]"));

	return s;
}

static uc_string frag_tag_debug(struct frag_tag t, unsigned level)
{
	uc_string s;

	s = debug_indent(level);
	uc_string_concat_tmp(s, uc_from_ascii_str("[tag \""));
	uc_string_concat(s, t.name);
	if (uc_strlen(t.param))
	{
		uc_string_concat_tmp(s, uc_from_ascii_str("\" \""));
		uc_string_concat(s, t.param);
	}
	uc_string_concat_tmp(s, uc_from_ascii_str("\"]"));
	if (t.children->size)
	{
		uc_string_append(s, uc_from_ascii('\n'));
		uc_string_concat_tmp(
		    s, frag_list_debug_with_indent(t.children, level + 1));
	}

	return s;
}

static uc_string frag_debug_with_indent(struct bbcode_frag *f, unsigned level)
{
	switch (f->type)
	{
	case TEXT:
		return frag_text_debug(f->text, level);
	case TAG:
		return frag_tag_debug(f->tag, level);
	default:
		fprintf(stderr, "BUG: invalid type");
		abort();
	}
}

uc_string frag_debug(struct bbcode_frag *f)
{
	return frag_debug_with_indent(f, 0);
}

static uc_string frag_list_debug_with_indent(struct bbcode_frag_list *fl,
					     unsigned		      level)
{
	uc_string s;
	size_t	  i;

	s = uc_string_new(0);

	for (i = 0; i < fl->size; ++i)
	{
		uc_string_concat_tmp(s,
				     frag_debug_with_indent(fl->fs[i], level));
		if (i != fl->size - 1)
			uc_string_append(s, uc_from_ascii('\n'));
	}

	return s;
}

uc_string frag_list_debug(struct bbcode_frag_list *fl)
{
	return frag_list_debug_with_indent(fl, 0);
}
