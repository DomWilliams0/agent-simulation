#include "helper.h"
#include "world/world.h"
#include "util/util.h"

#define CREATE_WORLD \
	struct world_parameters params = { \
		.chunk_width = 2, \
		.chunk_height = 2 \
	}; \
	struct world *w = world_create(&params);


UNIT_TEST(world_creation_destruction)
{
	UNUSED(state);

	// valid creation
	CREATE_WORLD;
	assert_non_null(w);
	assert_int_equal(2, world_get_chunk_width(w));
	assert_int_equal(2, world_get_chunk_height(w));
	assert_null(world_get_file_path(w));
	world_destroy(w);

	// zero
	params.chunk_width = 0;
	params.chunk_height = 10;
	assert_null(world_create(&params));
	params.chunk_width = 10;
	params.chunk_height = 0;
	assert_null(world_create(&params));
	params.chunk_width = 0;
	params.chunk_height = 0;
	assert_null(world_create(&params));

	// currently unsupported
	params.chunk_width = 10;
	params.chunk_height = 20;
	params.file_path = "not null";
	assert_null(world_create(&params));
}

UNIT_TEST(world_initial_state)
{
	UNUSED(state);

	struct world_parameters params = {400, 400, NULL};
	struct world *w = world_create(&params);

	// tile by tile
	for (unsigned int y = 0; y < params.chunk_height; ++y)
	{
		for (unsigned int x = 0; x < params.chunk_width; ++x)
		{
			assert_int_equal(TILE_BLANK, world_get_tile(w, x, y));
		}
	}

	// using array functions
	unsigned int chunk_count;
	struct chunk *chunk = world_get_chunk_array(w, &chunk_count);

	while (chunk_count--)
	{
		tile *tiles = world_get_chunk_tiles(chunk);
		unsigned int tile_count = CHUNK_SIZE * CHUNK_SIZE;
		while (tile_count--)
		{
			assert_int_equal(TILE_BLANK, tiles[tile_count]);
		}

		chunk++;
	}
}

UNIT_TEST(world_entity_creation)
{
	UNUSED(state);

	CREATE_WORLD;

	world_body b = world_create_entity(w);
	assert_non_null(b);

	// TODO you can do better than this

	world_destroy(w);
}

UNIT_TEST(world_tile_set_get)
{
	UNUSED(state);

	CREATE_WORLD;

	// valid
	assert_int_equal(TILE_BLANK, world_get_tile(w, 1, 1));
	world_set_tile(w, 1, 1, TILE_GRASS);
	assert_int_equal(TILE_GRASS, world_get_tile(w, 1, 1));

	// invalid
	assert_int_equal(TILE_BLANK, world_get_tile(w, 100, 100));
	world_set_tile(w, 100, 100, TILE_GRASS);
	assert_int_equal(TILE_BLANK, world_get_tile(w, 100, 100));

	world_destroy(w);
}

REGISTER_TEST(world_creation_destruction);
REGISTER_TEST(world_entity_creation);
REGISTER_TEST(world_tile_set_get);
REGISTER_TEST(world_initial_state);
