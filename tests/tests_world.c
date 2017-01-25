#include "helper.h"
#include "world/world.h"
#include "util/util.h"

#define CREATE_WORLD \
	struct world_parameters params = { \
		.width = 64, \
		.height = 96 \
	}; \
	struct world *w = world_create(&params);


UNIT_TEST(world_creation_destruction)
{
	UNUSED(state);

	// valid creation
	CREATE_WORLD;
	assert_non_null(w);
	assert_int_equal(64, world_get_width(w));
	assert_int_equal(96, world_get_height(w));
	assert_null(world_get_file_path(w));
	world_destroy(w);

	// zero
	params.width = 0;
	params.height = 32;
	assert_null(world_create(&params));
	params.width = 32;
	params.height = 0;
	assert_null(world_create(&params));
	params.width = 0;
	params.height = 0;
	assert_null(world_create(&params));

	// multiple of CHUNK_SIZE
	params.width = 33;
	params.height = 32;
	assert_null(world_create(&params));
	params.width = 32;
	params.height = 33;
	assert_null(world_create(&params));
	params.width = 55;
	params.height = 55;
	assert_null(world_create(&params));

	// negative
	params.width = -32;
	params.height = 32;
	assert_null(world_create(&params));
	params.width = 32;
	params.height = -32;
	assert_null(world_create(&params));
	params.width = -32;
	params.height = -32;
	assert_null(world_create(&params));

	// currently unsupported
	params.width = 40;
	params.height = 50;
	params.file_path = "not null";
	assert_null(world_create(&params));
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
