#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define safe_malloc(size, ptr) safe_malloc_wrapper(size, (void **)ptr, __FILE__, __LINE__)
#define safe_malloc_struct(struct_type, ptr) safe_malloc(sizeof(struct_type), ptr)

void safe_malloc_wrapper(size_t size, void **ptr, const char *file, unsigned int line);

void safe_free(void *ptr);

#endif
