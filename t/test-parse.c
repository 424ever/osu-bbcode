#include "parse.h"
#include "tap.h"
#include "unicode.h"

void uis(uc_string got, uc_string expected, const char *m, const char *name)
{
	int test;

	test = !uc_strcmp(got, expected);
	ok(test, m, name);
	if (!test)
	{
		diag("         got: '%s'", uc_to_ascii_str(got));
		diag("    expected: '%s'", uc_to_ascii_str(expected));
	}
}

void test_tag_attrs_ok(const char *name, uc_string input, uc_string exp_tagname,
		       uc_string exp_param, int exp_open)
{
	struct parser p;
	uc_string     act_tagname;
	uc_string     act_param;
	int	      act_open;
	int	      act_res;

	parser_init(&p, input);
	act_res = parse_tag_attrs(&p, &act_tagname, &act_param, &act_open);
	ok(act_res, "%s, res", name);
	cmp_ok(act_open, "==", exp_open, "%s, open", name);
	uis(act_tagname, exp_tagname, "%s, name", name);
	uis(act_param, exp_param, "%s, param", name);

	uc_string_free(exp_tagname);
	uc_string_free(exp_param);
	uc_string_free(act_tagname);
	uc_string_free(act_param);
	uc_string_free(input);
}
void test_tag_attrs_nok(const char *name, uc_string input)
{
	struct parser p;
	int	      act_res;

	parser_init(&p, input);
	act_res = parse_tag_attrs(&p, NULL, NULL, NULL);
	ok(!act_res, "%s, res", name);

	uc_string_free(input);
}

int main(void)
{
	plan(NO_PLAN);

	lives_ok(
	    {
		    struct parser p;
		    uc_string	  input;
		    input = uc_from_ascii_str("[abc]");
		    parser_init(&p, input);
		    (void) parse_tag_attrs(&p, NULL, NULL, NULL);
		    uc_string_free(input);
	    },
	    "tag null inputs");
	test_tag_attrs_ok("tag open", uc_from_ascii_str("[abc]"),
			  uc_from_ascii_str("abc"), uc_string_new(0), 1);
	test_tag_attrs_ok("tag param", uc_from_ascii_str("[abc=def]"),
			  uc_from_ascii_str("abc"), uc_from_ascii_str("def"),
			  1);
	test_tag_attrs_ok("tag empty param", uc_from_ascii_str("[abc=]"),
			  uc_from_ascii_str("abc"), uc_string_new(0), 1);
	test_tag_attrs_ok("tag close", uc_from_ascii_str("[/abc]"),
			  uc_from_ascii_str("abc"), uc_string_new(0), 0);
	test_tag_attrs_nok("tag close param", uc_from_ascii_str("[/abc=def]"));
	test_tag_attrs_nok("tag wrong start", uc_from_ascii_str("abc]"));
	test_tag_attrs_nok("tag no close", uc_from_ascii_str("[abc"));
	test_tag_attrs_nok("tag space name 1", uc_from_ascii_str("[a bc]"));
	test_tag_attrs_nok("tag space name 2", uc_from_ascii_str("[ abc]"));
	test_tag_attrs_nok("tag space name 3", uc_from_ascii_str("[abc =]"));
	test_tag_attrs_nok("tag name bracket", uc_from_ascii_str("[ab[]"));
	test_tag_attrs_nok("tag param bracket", uc_from_ascii_str("[abc=de[]"));

	done_testing();
}
