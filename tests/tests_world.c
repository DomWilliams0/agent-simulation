#include "helper.h"
#include "world/world.h"

UNIT_TEST(world_creation_destruction)
{
	struct world *w;

	world_create(&w);
	assert_non_null(w);

	world_destroy(&w);
	assert_null(w);
}

UNIT_TEST(world_entity_creation)
{
	struct world *w;
	world_create(&w);

	world_body b = world_create_entity(w);
	assert_non_null(b);

	// TODO you can do better than this

	world_destroy(&w);
}

REGISTER_TEST(world_creation_destruction);
REGISTER_TEST(world_entity_creation);
