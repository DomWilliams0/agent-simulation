#include <stddef.h>

#include "world/world.h"
#include "util/log.h"

BOOL world_save(struct world *w, const char * const file_path)
{
	if (file_path == NULL)
	{
		LOG_ERROR("Null world path");
		return FALSE;
	}

	return FALSE;
}

struct world *world_load(const char * const file_path)
{
	if (file_path == NULL)
	{
		LOG_ERROR("Null world path");
		return NULL;
	}

	return NULL;
}
