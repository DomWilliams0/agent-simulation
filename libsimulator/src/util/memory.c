#include <stdlib.h>

#include "util/log.h"
#include "util/memory.h"
#include "util/util.h"


void safe_malloc_wrapper(size_t size, void **ptr, const char *file, unsigned int line)
{
	void *p = NULL;

	if (size > 0)
	{
		LOG_DEBUG("Allocating %lu bytes at %s:%d", size, file, line);
		p = calloc(1, size);
	}

	if (!p)
	{
		LOG_ERROR("Failed to allocate %lu bytes at %s:%d", size, file, line);
		*ptr = NULL;
		exit(100);
	}

	*ptr = p;
}

void safe_free_wrapper(void *ptr, const char *file, unsigned int line)
{
	UNUSED(file);
	UNUSED(line);
	if (ptr)
	{
		LOG_DEBUG("Freeing %p at %s:%d", ptr, file, line);
		free(ptr);
	}
}
