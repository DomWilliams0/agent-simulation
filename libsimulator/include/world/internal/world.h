#ifndef WORLD_MODULE_H
#define WORLD_MODULE_H

#include <ode/ode.h>
#include <stdint.h>

#include "world/world.h"

struct world
{
	dWorldID phys_id;
	dSpaceID collision_space;
	dJointGroupID contacts;

	coord width;
	coord height;
};

#endif
