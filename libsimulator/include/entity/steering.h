#ifndef STEERING_H
#define STEERING_H

#include "world/world.h"

#define STEERING_ARRIVE_RADIUS (1.5f)

enum steering_type
{
	STEERING_SEEK = 1,
	STEERING_ARRIVE,
	STEERING_PATH_FOLLOW
};

struct entity_ctx;
struct world;
struct component_steer;

void steering_update_system(struct entity_ctx *entities, struct world *world);

void steering_apply(struct component_steer *steer, position current_pos, float *velocity);

#endif
