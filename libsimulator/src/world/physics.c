#include <entity/common.h>
#include "world/internal/physics.h"
#include "world/internal/world.h"
#include "world/world.h"

void create_physics_world(struct world *world)
{
	cpSpace *space = cpSpaceNew();
	cpSpaceSetGravity(space, cpv(0.0, 0.0));
	cpSpaceSetIterations(space, 10);
	cpSpaceSetDamping(space, WORLD_DAMPING);

	world->space = space;
	// TODO add world boundary
}

void destroy_physics_world(struct world *world)
{
	if (world->space)
	{
		cpSpaceFree(world->space);
		world->space = NULL;
	}
}

