#ifndef STEERING_H
#define STEERING_H

#include "world/world.h"

#define STEERING_ARRIVE_RADIUS (0.5f)

enum steering_type
{
	STEERING_SEEK = 1,
	STEERING_ARRIVE,
	STEERING_PATH_FOLLOW
};

struct steering_path_waypoint
{
	double pos[2];
	struct steering_path_waypoint *next;
};

struct entity_ctx;
struct world;
struct component_steer;

void steering_update_system(struct entity_ctx *entities);

void steering_apply(struct component_steer *steer, double current_pos[2], double *velocity);

void steering_path_add(struct component_steer *steer, double waypoint[2]);
bool steering_path_pop(struct component_steer *steer);
void steering_path_set(struct component_steer *steer, double *waypoints, uint32_t n);

#endif
