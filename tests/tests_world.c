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

REGISTER_TEST(world_creation_destruction);
