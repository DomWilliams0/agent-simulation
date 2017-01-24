#include "helper.h"
#include "world/world.h"
#include "util/util.h"

#define CREATE_WORLD \
	struct world_parameters params = { \
		.width = 40, \
		.height = 50 \
	}; \
	struct world *w = world_create(&params);


UNIT_TEST(world_creation_destruction)
{
	UNUSED(state);

	// valid creation
	CREATE_WORLD;
	assert_non_null(w);
	assert_int_equal(40, world_get_width(w));
	assert_int_equal(50, world_get_height(w));
	assert_null(world_get_file_path(w));
	world_destroy(w);

	// invalid size
	params.width = 0;
	params.height = 40;
	assert_null(world_create(&params));
	params.width = 40;
	params.height = 0;
	assert_null(world_create(&params));
	params.width = 0;
	params.height = 0;
	assert_null(world_create(&params));

	params.width = -20;
	params.height = 40;
	assert_null(world_create(&params));
	params.width = 40;
	params.height = -20;
	assert_null(world_create(&params));
	params.width = -10;
	params.height = -10;
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

REGISTER_TEST(world_creation_destruction);
REGISTER_TEST(world_entity_creation);
