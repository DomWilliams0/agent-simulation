#ifndef WORLD_PHYSICS_H
#define WORLD_PHYSICS_H

#include <ode/ode.h>
#include "world/world.h"

void create_physics_world(struct world *world);

void destroy_physics_world(struct world *world);

void near_callback(void *data, dGeomID o1, dGeomID o2);

#endif
