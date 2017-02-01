#ifndef STEERING_H
#define STEERING_H

#include "world/world.h"
#include "util/bool.h"

#define STEERING_ARRIVE_RADIUS (1.5f)

enum steering_type
{
	STEERING_SEEK = 1,
	STEERING_ARRIVE,
	STEERING_PATH_FOLLOW
};

struct steering_path_waypoint
{
	float pos[2];
	struct steering_path_waypoint *next;
};

struct entity_ctx;
struct world;
struct component_steer;

void steering_update_system(struct entity_ctx *entities, struct world *world);

void steering_apply(struct component_steer *steer, float current_pos[2], float *velocity);

void steering_path_add(struct component_steer *steer, float waypoint[2]);
BOOL steering_path_pop(struct component_steer *steer, float out[2]);
void steering_path_set(struct component_steer *steer, float *waypoints, unsigned int n);

#endif
