#include "parse.h"
#include "tap.h"
#include "unicode.h"

void uis(uc_string got, uc_string expected, const char *m)
{
	int test;

	test = !uc_strcmp(got, expected);
	ok(test, m);
	if (!test)
	{
		diag("         got: '%s'", uc_to_ascii_str(got));
		diag("    expected: '%s'", uc_to_ascii_str(expected));
	}
}

int main(void)
{
	uc_string	  s1;
	uc_string	  s2;
	uc_string	  s3;
	uc_string	  empty_string;
	struct parser	  p;
	struct tag_result t;

	plan(NO_PLAN);

	empty_string = uc_from_ascii_str("");

	/* tag open */
	s1 = uc_from_ascii_str("[abc]");
	s2 = uc_from_ascii_str("abc");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(t.ok, "tag open, ok");
	ok(t.success.open, "tag open, open");
	uis(t.success.tag_name, s2, "tag open, name");
	uis(t.success.addit, empty_string, "tag open, addit");
	uc_string_free(s1);
	uc_string_free(s2);

	/* tag addit */
	s1 = uc_from_ascii_str("[abc=def]");
	s2 = uc_from_ascii_str("abc");
	s3 = uc_from_ascii_str("def");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(t.ok, "tag addit, ok");
	ok(t.success.open, "tag addit, open");
	uis(t.success.tag_name, s2, "tag addit, name");
	uis(t.success.addit, s3, "tag addit, addit");
	uc_string_free(s1);
	uc_string_free(s2);

	/* tag close */
	s1 = uc_from_ascii_str("[/abc]");
	s2 = uc_from_ascii_str("abc");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(t.ok, "tag close, ok");
	ok(!t.success.open, "tag close, open");
	uis(t.success.tag_name, s2, "tag close, name");
	uis(t.success.addit, empty_string, "tag close, addit");
	uc_string_free(s1);
	uc_string_free(s2);

	/* tag close addit */
	s1 = uc_from_ascii_str("[/abc=def]");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(!t.ok, "tag close addit, ok");
	uc_string_free(s1);

	/* tag wrong start */
	s1 = uc_from_ascii_str("abc]");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(!t.ok, "tag wrong start, ok");
	uc_string_free(s1);

	/* tag no close */
	s1 = uc_from_ascii_str("[abc");
	parser_init(&p, s1);
	t = parse_tag(&p);
	ok(!t.ok, "tag wrong start, ok");
	uc_string_free(s1);

	uc_string_free(empty_string);

	done_testing();
}
