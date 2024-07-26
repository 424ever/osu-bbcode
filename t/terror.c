#include "tap.h"
#include "unicode.h"

int main(void)
{
	plan(NO_PLAN);

	is(uc_last_error(), "", "empty at start");

	(void) uc_from_ascii((char) 128);
	isnt(uc_last_error(), "", "set after error");

	uc_unset_error_();
	is(uc_last_error(), "", "empty after unset");

	done_testing();
}
