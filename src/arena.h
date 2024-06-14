#include <stdlib.h>

#ifndef ARENA_H
struct alloc_arena;

struct alloc_arena *arena_new(void);
void		    arena_destroy(struct alloc_arena *);
void		   *arena_alloc(struct alloc_arena *, size_t);

#endif // !ARENA_H
