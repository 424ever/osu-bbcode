#include <stdlib.h>

#ifndef ALLOC_H
#define ALLOC_H

/*
 * An allocator which prints an error message and abort()s the program when
 * allocation fails.
 */
void *safe_alloc(const char *callsite, size_t nmemb, size_t size);
#endif
