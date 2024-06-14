#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.h"

#define ARENA_MAGIC 0x42

struct alloc_arena
{
	int	 magic;
	void   **allocs;
	uint32_t count;
};

struct alloc_arena *arena_new(void)
{
	struct alloc_arena *a;

	a = malloc(sizeof(*a));
	if (a == NULL)
	{
		fprintf(stderr, "Allocation of new arena failed.");
		abort();
	}

	a->magic  = ARENA_MAGIC;
	a->count  = 0;
	a->allocs = NULL;

	return a;
}

void arena_destroy(struct alloc_arena *a)
{
	if (a->magic != ARENA_MAGIC)
	{
		fprintf(stderr, "Attempted to destroy uninitialized arena.");
		abort();
	}
	for (uint32_t i = 0; i < a->count; ++i)
		free(a->allocs[i]);
	free(a->allocs);
	free(a);
}

void *arena_alloc(struct alloc_arena *a, size_t sz)
{
	a->count++;
	a->allocs = realloc(a->allocs, a->count * sizeof(*a->allocs));

	if (a->allocs == NULL)
	{
		fprintf(stderr, "Allocation of %" PRIu64 "bytes failed.", sz);
		abort();
	}

	a->allocs[a->count - 1] = malloc(sz);

	if (a->allocs[a->count - 1] == NULL)
	{
		fprintf(stderr, "Allocation of %" PRIu64 "bytes failed.", sz);
		abort();
	}

	return a->allocs[a->count - 1];
}
