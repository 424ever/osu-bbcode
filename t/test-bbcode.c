#include "./common.h"
#include "bbcode.h"
#include "tap.h"

static void test_debug_frag(struct bbcode_frag *f, uc_string exp, const char *n)
{
	uc_string act;

	act = frag_debug(f);
	uis(act, exp, n);
	frag_free(f);
	uc_string_free(exp);
	uc_string_free(act);
}

static void test_debug_list(struct bbcode_frag_list *fl, uc_string exp,
			    const char *n)
{
	uc_string act;

	act = frag_list_debug(fl);
	uis(act, exp, n);
	frag_list_free(fl);
	uc_string_free(exp);
	uc_string_free(act);
}

static void test_debug_doc(struct bbcode_doc *d, uc_string exp, const char *n)
{
	uc_string act;

	act = doc_debug(d);
	uis(act, exp, n);
	bbcode_doc_free(d);
	uc_string_free(exp);
	uc_string_free(act);
}

int main()
{
	struct bbcode_frag	*f;
	struct bbcode_frag_list *fl;
	struct bbcode_doc	*d;

	plan(NO_PLAN);

	/* debug */
	test_debug_frag(frag_text_new(uc_from_ascii_str("abc")),
			uc_from_ascii_str("[text \"abc\"]"), "debug text");

	test_debug_frag(
	    frag_tag_new(uc_from_ascii_str("a"), uc_from_ascii_str("")),
	    uc_from_ascii_str("[tag \"a\"]"), "debug tag no param");

	test_debug_frag(
	    frag_tag_new(uc_from_ascii_str("a"), uc_from_ascii_str("param")),
	    uc_from_ascii_str("[tag \"a\" \"param\"]"), "debug tag param");

	f = frag_tag_new(uc_from_ascii_str("a"), uc_from_ascii_str(""));
	frag_tag_append(f, frag_text_new(uc_from_ascii_str("text 1")));
	frag_tag_append(
	    f, frag_tag_new(uc_from_ascii_str("b"), uc_from_ascii_str("")));
	frag_tag_append(
	    f, frag_tag_new(uc_from_ascii_str("c"), uc_from_ascii_str("p")));
	test_debug_frag(f,
			uc_from_ascii_str("[tag \"a\"]\n"
					  "  [text \"text 1\"]\n"
					  "  [tag \"b\"]\n"
					  "  [tag \"c\" \"p\"]"),
			"debug tag no param children");

	f = frag_tag_new(uc_from_ascii_str("a"), uc_from_ascii_str("p"));
	frag_tag_append(f, frag_text_new(uc_from_ascii_str("text 1")));
	frag_tag_append(
	    f, frag_tag_new(uc_from_ascii_str("b"), uc_from_ascii_str("")));
	frag_tag_append(
	    f, frag_tag_new(uc_from_ascii_str("c"), uc_from_ascii_str("p")));
	test_debug_frag(f,
			uc_from_ascii_str("[tag \"a\" \"p\"]\n"
					  "  [text \"text 1\"]\n"
					  "  [tag \"b\"]\n"
					  "  [tag \"c\" \"p\"]"),
			"debug tag param children");

	fl = frag_list_new();
	test_debug_list(fl, uc_string_new(0), "debug frag list empty");

	fl = frag_list_new();
	frag_list_append(fl, frag_text_new(uc_from_ascii_str("text 1")));
	frag_list_append(
	    fl, frag_tag_new(uc_from_ascii_str("b"), uc_from_ascii_str("")));
	frag_list_append(
	    fl, frag_tag_new(uc_from_ascii_str("c"), uc_from_ascii_str("p")));
	test_debug_list(fl,
			uc_from_ascii_str("[text \"text 1\"]\n"
					  "[tag \"b\"]\n"
					  "[tag \"c\" \"p\"]"),
			"debug frag list full");

	d = bbcode_doc_new();
	test_debug_doc(d, uc_from_ascii_str("<root>"), "debug frag list empty");

	d = bbcode_doc_new();
	doc_append(d, frag_text_new(uc_from_ascii_str("text 1")));
	doc_append(d,
		   frag_tag_new(uc_from_ascii_str("b"), uc_from_ascii_str("")));
	doc_append(
	    d, frag_tag_new(uc_from_ascii_str("c"), uc_from_ascii_str("p")));
	test_debug_doc(d,
		       uc_from_ascii_str("<root>\n"
					 "  [text \"text 1\"]\n"
					 "  [tag \"b\"]\n"
					 "  [tag \"c\" \"p\"]"),
		       "debug frag list full");

	done_testing();
}
