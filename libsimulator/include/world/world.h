#ifndef WORLD_H
#define WORLD_H

#include "world/world_forward.h"
#include "util/util.h"

#define HUMAN_RADIUS (0.3)
#define HUMAN_MASS   (10.0)

struct position
{
	coord x, y;
};

MODULE_DECLARE(struct world, world_create, world_destroy)

void world_step(struct world *w);

world_body world_create_entity(struct world *w);

void world_get_position(world_body body, struct position *pos);

void world_set_position(world_body body, struct position *pos);

#endif
