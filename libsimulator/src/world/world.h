#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stddef.h>
#include <stdbool.h>
#include <chipmunk/chipmunk.h>
#include <util/vec.h>

#include "road.h"
#include "world/world.h"
#include "util/util.h"

#define HUMAN_MASS (60.0) // kg

typedef cpBody *world_body;
typedef int coord; // TODO remove this
typedef vec_t(struct road) vec_road;

struct world_parameters
{
	coord width;
	coord height;
};

MOD_DECLARE(world, {
	cpSpace *space;
	vec_road roads;

	coord width;
	coord height;
})

void world_step(struct world *w);

// entities
world_body world_create_entity(struct world *w);
cpVect world_get_position(world_body body);
void world_set_position(world_body body, cpVect pos);

cpVect world_get_velocity(world_body body);

// roads
void world_add_road(struct world *w, struct road *road);
#endif
