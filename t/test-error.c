#include "error.h"
#include "tap.h"
#include "unicode.h"

int main(void)
{
	plan(NO_PLAN);

	ok(!error_occured(), "empty at start set");
	is(get_error(), "", "empty at start str");

	(void) uc_from_ascii((char) 128);
	ok(error_occured(), "set after error set");
	isnt(get_error(), "", "set after error str");

	unset_error();
	ok(!error_occured(), "empty after unset set");
	is(get_error(), "", "empty after unset str");

	done_testing();
}
