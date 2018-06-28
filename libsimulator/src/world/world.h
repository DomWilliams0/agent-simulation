#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stddef.h>
#include <stdbool.h>
#include <chipmunk/cpVect.h>

#include "world/world_forward.h"
#include "util/util.h"

#define HUMAN_MASS (60.0) // kg

struct world_parameters
{
	coord width;
	coord height;
};

MOD_FWD_DECLARE(world)

void world_step(struct world *w);

// entities
world_body world_create_entity(struct world *w);
cpVect world_get_position(world_body body);
void world_set_position(world_body body, cpVect pos);

cpVect world_get_velocity(world_body body);

#endif
