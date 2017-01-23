#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <memory.h>

#define safe_malloc(size, ptr) safe_malloc_wrapper(size, (void **)ptr, __FILE__, __LINE__)
#define safe_malloc_struct(struct_type, ptr) safe_malloc(sizeof(struct_type), ptr)


#define SWAP_IN_ARRAY(type, arr, i, j) SWAP_IN_ARRAY_IMP(MACRO_CONCAT(temp, __COUNTER__), sizeof(type), arr, i, j)

#define SWAP_IN_ARRAY_IMP(temp_name, size, arr, i, j) \
	char temp_name[size]; \
	memcpy(temp_name, arr + i, size); \
	memcpy(arr + i, arr + j, size); \
	memcpy(arr + j, temp_name, size);


void safe_malloc_wrapper(size_t size, void **ptr, const char *file, unsigned int line);

void safe_free(void *ptr);

#endif
