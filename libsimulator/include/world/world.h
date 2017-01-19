#ifndef WORLD_H
#define WORLD_H

#include "world/world_forward.h"

struct world;

struct position
{
	coord x, y;
};

struct world *world_create();

void world_destroy(struct world *w);

void world_step(struct world *w);

world_body world_create_entity(struct world *w);

void world_get_position(world_body body, struct position *pos);

void world_set_position(world_body body, struct position *pos);

#endif
