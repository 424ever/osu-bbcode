#include "alloc.h"
#include "tap.h"
#include <stdint.h>

int main(void)
{
	void *p;
	plan(NO_PLAN);

	p = safe_alloc("test", 1, 1);
	ok(!!p, "small alloc");
	free(p);

	dies_ok({ safe_alloc("test", SIZE_MAX, SIZE_MAX); }, "fail crashes");

	p = safe_alloc("test", 1, 1);
	p = safe_realloc("test", p, 1, 2);
	ok(!!p, "realloc ok");
	free(p);

	dies_ok(
	    {
		    p = safe_alloc("test", 1, 1);
		    safe_realloc("test", p, SIZE_MAX, SIZE_MAX);
	    },
	    "realloc crashes");

	done_testing();
}
