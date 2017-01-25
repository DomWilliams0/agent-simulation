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

	int width;
	int height;
	char *file_path;

	struct chunk *chunks;
};

struct chunk
{
	uint8_t tiles[CHUNK_SIZE * CHUNK_SIZE];
};

#endif
