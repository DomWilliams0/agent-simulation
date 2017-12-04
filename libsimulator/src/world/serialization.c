#include <stdio.h>

#include "tpl/tpl.h"
#include "world/internal/world.h"
#include "util/log.h"

// TODO magic and version

#define TPL_HEADER "S(uu)" // dimensions
#define TPL_BODY   "u#"    // raw tiles

struct serialize_header
{
	chunk_coord chunk_width;
	chunk_coord chunk_height;
};

struct serialize_body
{
	struct chunk *chunks;
};

BOOL world_save(struct world *w, const char * const file_path)
{
	if (file_path == NULL)
	{
		LOG_ERROR("Null world path");
		return FALSE;
	}

	struct serialize_header header = {
		.chunk_width = w->chunk_width,
		.chunk_height = w->chunk_height
	};

	struct serialize_body body = {
		.chunks = w->chunks
	};

	tpl_node *tpl_header, *tpl_body;
	BOOL success = FALSE;

	// header
	if ((tpl_header = tpl_map(TPL_HEADER, &header)) != NULL)
	{
		if (tpl_pack(tpl_header, 0) == 0)
		{
			// body
			unsigned long tile_count = header.chunk_width * header.chunk_height * CHUNK_TILE_COUNT;
			if ((tpl_body = tpl_map(TPL_BODY, body.chunks, tile_count)) != NULL)
			{
				if (tpl_pack(tpl_body, 0) == 0)
				{
					// write both to file
					FILE *f = fopen(file_path, "wb");
					if (f != NULL)
					{
						int fd = fileno(f);
						if (tpl_dump(tpl_header, TPL_FD, fd) == 0 && tpl_dump(tpl_body, TPL_FD, fd) == 0)
						{
							success = TRUE;
						}
					}
				}

				tpl_free(tpl_body);
			}
		}

		tpl_free(tpl_header);
	}

	return success;
}

struct world *world_load(const char * const file_path)
{
	if (file_path == NULL)
	{
		LOG_ERROR("Null world path");
		return NULL;
	}

	struct serialize_header header;
	struct serialize_body body;

	struct world *world = NULL;
	tpl_node *tpl_header, *tpl_body;
	BOOL success = FALSE;

	// load header
	if ((tpl_header = tpl_map(TPL_HEADER, &header)) != NULL)
	{
		FILE *f = fopen(file_path, "rb");
		if (f != NULL)
		{
			int fd = fileno(f);
			if (tpl_load(tpl_header, TPL_FD, fd) == 0 && tpl_unpack(tpl_header, 0))
			{
				// allocate world
				struct world_parameters params = {header.chunk_width, header.chunk_height};
				if ((world = world_create(&params)) != NULL)
				{
					body.chunks = world->chunks;

					// load body
					unsigned long tile_count = header.chunk_width * header.chunk_height * CHUNK_TILE_COUNT;
					if ((tpl_body = tpl_map(TPL_BODY, body.chunks, tile_count)) != NULL)
					{
						if (tpl_load(tpl_body, TPL_FD, fd) == 0 && tpl_unpack(tpl_body, 0))
						{
							success = TRUE;
						}

						tpl_free(tpl_body);
					}
				}

				fclose(f);
			}

		}
	}

	if (!success && world)
	{
		world_destroy(world);
		world = NULL;
	}

	return world;
}
