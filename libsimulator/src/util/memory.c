#include <stdlib.h>

#include "util/log.h"
#include "util/memory.h"


void safe_malloc_wrapper(size_t size, void **ptr, const char *file, unsigned int line)
{
	void *p = calloc(1, size);
	if (!p)
	{
		LOG_INFO("Failed to allocate %lu bytes at %s:%d", size, file, line);
		*ptr = NULL;
		exit(100);
	}

	*ptr = p;
}

void safe_free(void *ptr)
{
	if (ptr)
		free(ptr);
}
