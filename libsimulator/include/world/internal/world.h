#ifndef WORLD_MODULE_H
#define WORLD_MODULE_H

#include <ode/ode.h>
#include <stdint.h>

#include "world/world.h"

struct world
{
	world_id id;

	dWorldID phys_id;
	dSpaceID collision_space;
	dJointGroupID contacts;

	uint32_t chunk_width;
	uint32_t chunk_height;

	struct chunk *chunks;
};

#endif
