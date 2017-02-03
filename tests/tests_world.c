#include <stdio.h>

#include "helper.h"

#include "world/world.h"
#include "util/util.h"

#define CREATE_WORLD \
	struct world_parameters params = { \
		.chunk_width = 2, \
		.chunk_height = 7 \
	}; \
	struct world *w = world_create(&params);


UNIT_TEST(world_creation_destruction)
{
	UNUSED(state);

	// valid creation
	CREATE_WORLD;
	assert_non_null(w);
	assert_int_equal(2, world_get_chunk_width(w));
	assert_int_equal(7, world_get_chunk_height(w));
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
}

UNIT_TEST(world_initial_state)
{
	UNUSED(state);

	struct world_parameters params = {40, 40};
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

UNIT_TEST(world_serialization)
{
	// non existent file
	assert_null(world_load(NULL));
	assert_null(world_load("non existent"));

	// create and populate
	struct world_parameters params = {4, 4};
	struct world *w = world_create(&params);

	const int check_count = 6;
	float tiles[12] = {0, 0, 1, 0, 2, 0, 3, 0, 100, 10, 20, 60};
	for (int i = 0; i < check_count * 2; i += 2)
		world_set_tile(w, tiles[i], tiles[i + 1], TILE_GRASS);

	// dump to file
	const char * const path = "/tmp/test-world";
	assert_false(world_save(w, NULL));
	assert_true(world_save(w, path));

	// read back
	struct world *loaded = world_load(path);
	assert_non_null(loaded);

	for (int i = 0; i < check_count; i += 2)
		assert_int_equal(TILE_GRASS, world_get_tile(w, tiles[i], tiles[i + 1]));

	world_destroy(w);
	world_destroy(loaded);

	// delete temp file
	remove(path);
}

REGISTER_TEST(world_creation_destruction);
REGISTER_TEST(world_entity_creation);
REGISTER_TEST(world_tile_set_get);
REGISTER_TEST(world_initial_state);
REGISTER_TEST(world_serialization);
