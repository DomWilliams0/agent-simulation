#ifndef SIM_WORLD_PHYSICS_H
#define SIM_WORLD_PHYSICS_H

#include <chipmunk/chipmunk.h>

#include "world/world.h"

void create_physics_world(struct world *world);

void destroy_physics_world(struct world *world);

#endif
