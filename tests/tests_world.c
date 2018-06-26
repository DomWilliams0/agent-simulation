#include <stdio.h>

#include "tests.h"

#include "world/world.h"
#include "util/memory.h"
#include "util/util.h"

void test_world()
{
	struct world *world;
	safe_malloc(world_sizeof(), &world);
	ASSERT(world_init(world, NULL) == 1);

	struct world_parameters params = {0};
	ASSERT(world_init(world, &params) == 1);

	params.width = 40;
	params.height = 40;
	ASSERT(world_init(world, &params) == 0);

	// add entity
	world_body e = world_create_entity(world);
	ASSERT(e != NULL);

	double pos[2] = {5.0, 25.0};
	world_set_position(e, &pos[0]);

	double pos_get[2];
	world_get_position(e, pos_get);
	ASSERT_POS(pos, pos_get);

	safe_free(world);
}
