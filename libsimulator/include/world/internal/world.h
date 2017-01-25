#ifndef WORLD_MODULE_H
#define WORLD_MODULE_H

#include <ode/ode.h>
#include <stdint.h>

#include "world/world.h"

struct world
{
	unsigned int id;

	dWorldID phys_id;
	dSpaceID collision_space;
	dJointGroupID contacts;

	unsigned int chunk_width;
	unsigned int chunk_height;
	char *file_path;

	struct chunk *chunks;
};

#endif
