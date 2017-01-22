#include "helper.h"
#include "world/world.h"
#include "util/util.h"

UNIT_TEST(world_creation_destruction)
{
	UNUSED(state);
	struct world *w = world_create(NULL);
	assert_non_null(w);

	world_destroy(w);
}

UNIT_TEST(world_entity_creation)
{
	UNUSED(state);

	struct world *w = world_create(NULL);

	world_body b = world_create_entity(w);
	assert_non_null(b);

	// TODO you can do better than this

	world_destroy(w);
}

REGISTER_TEST(world_creation_destruction);
REGISTER_TEST(world_entity_creation);
