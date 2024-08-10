#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"

void *safe_alloc(const char *callsite, size_t nmemb, size_t size)
{
	void *p;

	p = calloc(nmemb, size);
	if (p == NULL)
	{
		fprintf(stderr,
			"Allocation of %zd elements, %zd bytes each, failed at "
			"%s.\n",
			nmemb, size, callsite);
		abort();
	}

	return p;
}

void *safe_realloc(const char *callsite, void *p, size_t nmemb, size_t size)
{
	p = reallocarray(p, nmemb, size);
	if (p == NULL)
	{
		fprintf(
		    stderr,
		    "Reallocation of %zd elements, %zd bytes each, failed at "
		    "%s.\n",
		    nmemb, size, callsite);
		abort();
	}
	return p;
}
