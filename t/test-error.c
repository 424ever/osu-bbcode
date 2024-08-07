#include "tap.h"
#include "unicode.h"

int main(void)
{
	plan(NO_PLAN);

	ok(!uc_is_err_set(), "empty at start set");
	is(uc_last_error(), "", "empty at start str");

	(void) uc_from_ascii((char) 128);
	ok(uc_is_err_set(), "set after error set");
	isnt(uc_last_error(), "", "set after error str");

	uc_unset_error_();
	ok(!uc_is_err_set(), "empty after unset set");
	is(uc_last_error(), "", "empty after unset str");

	done_testing();
}
