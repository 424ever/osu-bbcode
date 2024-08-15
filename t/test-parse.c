#include "./common.h"
#include "error.h"
#include "parse.h"
#include "tap.h"
#include "unicode.h"

static void test_text_ok(const char *name, const char *input,
			 const char *exp_output)
{
	struct parser p;
	uc_string     uin;
	uc_string     uex;
	uc_string     uac;
	int	      res;

	uin = uc_from_ascii_str(input);
	uex = uc_from_ascii_str(exp_output);

	parser_init(&p, uin);
	res = parse_text(&p, &uac);

	ok(res, "%s, res", name);
	ok(!!uac, "%s, nonnull", name);
	if (res && !!uac)
	{
		uis_n(uac, uex, "%s, text", name);
		uc_string_free(uac);
	}
	else
	{
		fail("%s, text", name);
	}

	uc_string_free(uex);
	uc_string_free(uin);
}

static void test_tag_attrs_ok(const char *name, const char *input,
			      const char *exp_tagname, const char *exp_param,
			      int exp_open)
{
	struct parser p;
	uc_string     act_tagname;
	uc_string     act_param;
	int	      act_open;
	int	      act_res;
	uc_string     uinput, uexp_tagname, uexp_param;

	uinput	     = uc_from_ascii_str(input);
	uexp_tagname = uc_from_ascii_str(exp_tagname);
	uexp_param   = uc_from_ascii_str(exp_param);

	parser_init(&p, uinput);
	act_res = parse_tag_attrs(&p, &act_tagname, &act_param, &act_open);
	ok(act_res, "%s, res", name);
	cmp_ok(act_open, "==", exp_open, "%s, open", name);
	uis_n(act_tagname, uexp_tagname, "%s, name", name);
	uis_n(act_param, uexp_param, "%s, param", name);

	uc_string_free(uexp_tagname);
	uc_string_free(uexp_param);
	uc_string_free(act_tagname);
	uc_string_free(act_param);
	uc_string_free(uinput);
}

static void test_tag_attrs_nok(const char *name, const char *input)
{
	struct parser p;
	int	      act_res;
	uc_string     uinput;

	uinput = uc_from_ascii_str(input);

	parser_init(&p, uinput);
	act_res = parse_tag_attrs(&p, NULL, NULL, NULL);
	ok(!act_res, "%s, res", name);

	uc_string_free(uinput);
}

static void test_frag_ok(const char *name, const char *input, const char *debug)
{
	struct parser	    p;
	uc_string	    act;
	struct bbcode_frag *frag;
	int		    res;
	uc_string	    uinput;
	uc_string	    udebug;

	uinput = uc_from_ascii_str(input);
	udebug = uc_from_ascii_str(debug);

	parser_init(&p, uinput);
	res = parse_frag(&p, &frag);
	if (!res)
	{
		fail("%s succesful", name);
		diag("error: %s", get_error());
	}
	else
	{
		act = frag_debug(frag);
		uis(act, udebug, name);
		uc_string_free(act);
		frag_free(frag);
	}

	uc_string_free(udebug);
	uc_string_free(uinput);
}

static void test_frag_nok(const char *name, const char *input)
{
	struct parser p;
	uc_string     uinput;
	int	      res;

	uinput = uc_from_ascii_str(input);

	parser_init(&p, uinput);

	res = parse_frag(&p, NULL);
	ok(!res, name);

	uc_string_free(uinput);
}

static void test_frag_list_ok(const char *name, const char *input,
			      const char *debug)
{
	int			 res;
	struct bbcode_frag_list *l;
	struct parser		 p;
	uc_string		 act;
	uc_string		 udebug;
	uc_string		 uinput;

	uinput = uc_from_ascii_str(input);
	udebug = uc_from_ascii_str(debug);

	parser_init(&p, uinput);

	res = parse_frag_list(&p, &l);
	if (!res)
	{
		fail("%s succesful", name);
		diag("error: %s", get_error());
	}
	else
	{
		act = frag_list_debug(l);
		uis(act, udebug, name);
		uc_string_free(act);
		frag_list_free(l);
	}

	uc_string_free(uinput);
	uc_string_free(udebug);
}

static void test_frag_list_nok(const char *name, const char *input)
{
	struct parser p;
	uc_string     uinput;
	int	      res;

	uinput = uc_from_ascii_str(input);

	parser_init(&p, uinput);

	res = parse_frag_list(&p, NULL);
	ok(!res, name);

	uc_string_free(uinput);
}

int main(void)
{
	plan(NO_PLAN);

	/* text */
	test_text_ok("text whole text", "abcde", "abcde");
	test_text_ok("text stray open bracket", "ab[cd", "ab[cd");
	test_text_ok("text but actually tag", "[abc]", "");
	test_text_ok("text tag at end", "abc [def]", "abc ");
	test_text_ok("text tag middle", "abc [def] ghi[/def]", "abc ");
	test_text_ok("text double bracket", "abc [[def]", "abc [");

	/* tag attrs */
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
	test_tag_attrs_ok("tag open", "[abc]", "abc", "", 1);
	test_tag_attrs_ok("tag param", "[abc=def]", "abc", "def", 1);
	test_tag_attrs_ok("tag empty param", "[abc=]", "abc", "", 1);
	test_tag_attrs_ok("tag close", "[/abc]", "abc", "", 0);
	test_tag_attrs_nok("tag close param", "[/abc=def]");
	test_tag_attrs_nok("tag wrong start", "abc]");
	test_tag_attrs_nok("tag no close", "[abc");
	test_tag_attrs_nok("tag space name 1", "[a bc]");
	test_tag_attrs_nok("tag space name 2", "[ abc]");
	test_tag_attrs_nok("tag space name 3", "[abc =]");
	test_tag_attrs_nok("tag name bracket", "[ab[]");
	test_tag_attrs_nok("tag param bracket", "[abc=de[]");

	/* frag */
	test_frag_ok("frag only text", "abcde", "[text \"abcde\"]");
	test_frag_ok("frag tag after frag", "abcde[tag][/tag]",
		     "[text \"abcde\"]");
	test_frag_ok("frag empty tag", "[a][/a]", "[tag \"a\"]");
	test_frag_ok("frag tag with text", "[a]abc[/a]",
		     "[tag \"a\"]\n"
		     "  [text \"abc\"]");
	test_frag_ok("frag nested tags", "[a]text 1[b=3]text 2[/b]text 3[/a]",
		     "[tag \"a\"]\n"
		     "  [text \"text 1\"]\n"
		     "  [tag \"b\" \"3\"]\n"
		     "    [text \"text 2\"]\n"
		     "  [text \"text 3\"]");
	test_frag_nok("frag no close", "[a]abc");
	test_frag_nok("frag wrong close order", "[a][b]text[/a][/b]");
	lives_ok(
	    {
		    struct parser p;
		    uc_string	  input;
		    input = uc_from_ascii_str("[a][/a]");
		    parser_init(&p, input);
		    (void) parse_frag(&p, NULL);
		    uc_string_free(input);
	    },
	    "frag null inputs");
	test_frag_nok("frag closed tag", "[/a]");

	/* frag list */
	lives_ok(
	    {
		    struct parser p;
		    uc_string	  input;
		    input = uc_from_ascii_str("[a][/a]abc");
		    parser_init(&p, input);
		    (void) parse_frag_list(&p, NULL);
		    uc_string_free(input);
	    },
	    "frag list null inputs");
	test_frag_list_ok("frag list ok", "t1[a=42]t2[b]t3[/b]t4[/a]t5",
			  "[text \"t1\"]\n"
			  "[tag \"a\" \"42\"]\n"
			  "  [text \"t2\"]\n"
			  "  [tag \"b\"]\n"
			  "    [text \"t3\"]\n"
			  "  [text \"t4\"]\n"
			  "[text \"t5\"]");
	test_frag_list_nok("frag list too much closing", "[a][/a][/b]");
	test_frag_list_nok("frag list unclosed", "[a][/a][b]");
	// "[a][/a][a]" => error

	done_testing();
}
