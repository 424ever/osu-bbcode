#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.h"

#define ARENA_MAGIC 0x42

struct alloc_arena
{
	int	 magic;
	uint64_t sz;
	uint8_t *d;
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

	a->magic = ARENA_MAGIC;
	a->sz	 = 0;
	a->d	 = NULL;

	return a;
}

void arena_destroy(struct alloc_arena *a)
{
	if (a->magic != ARENA_MAGIC)
	{
		fprintf(stderr, "Attempted to destroy uninitialized arena.");
		abort();
	}
	free(a->d);
	free(a);
}

void *arena_alloc(struct alloc_arena *a, uint64_t sz)
{
	uint64_t oldsz;

	oldsz = a->sz;

	a->sz += sz;
	a->d = realloc(a->d, a->sz);

	if (a->d == NULL)
	{
		fprintf(stderr, "Allocation of %" PRIu64 "bytes failed.", sz);
		abort();
	}

	return a->d + oldsz;
}
