#include <ode/ode.h>
#include <math.h>

#include "entity/steering.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/log.h"
#include "util/memory.h"

void steering_update_system(struct entity_ctx *entities)
{
	struct component_physics *physics = (struct component_physics *)entity_get_component_array(entities, COMPONENT_PHYSICS);
	struct component_steer *steers = (struct component_steer *)entity_get_component_array(entities, COMPONENT_STEER);

	entity_id count = entity_get_count(entities);
	const entity_mask render_mask = COMPONENT_PHYSICS | COMPONENT_STEER;

	float velocity[2];
	float position[2];
	for (entity_id i = 0; i < count; ++i)
	{
		if (!entity_has_component(entities, i, render_mask))
			continue;

		struct component_physics *phys = physics + i;
		world_get_position(phys->body, position);


		velocity[0] = velocity[1] = 0.f;
		struct component_steer *steer = steers + i;
		steering_apply(steer, position, velocity);

		dBodyAddForce(phys->body, velocity[0], velocity[1], 0);
	}

}

static double length(float *v)
{
	return sqrt((v[0] * v[0]) + (v[1] * v[1]));
}

static void scale(float *velocity, float speed)
{
	double l = length(velocity);
	if (l < VELOCITY_MINIMUM)
		l = 1;

	velocity[0] *= speed / l;
	velocity[1] *= speed / l;
}

static inline void handle_seek(float pos[2], float goal_x, float goal_y, float *velocity)
{
	// full speed ahead
	velocity[0] = goal_x - pos[0];
	velocity[1] = goal_y - pos[1];
	scale(velocity, HUMAN_ACCELERATION);
}

static inline void handle_arrive(float pos[2], float goal_x, float goal_y, float *velocity)
{
	velocity[0] = goal_x - pos[0];
	velocity[1] = goal_y - pos[1];

	double distance = length(velocity);
	float speed = HUMAN_ACCELERATION;
	if (distance < STEERING_ARRIVE_RADIUS)
		speed *= distance / STEERING_ARRIVE_RADIUS;

	scale(velocity, speed);
}

void steering_apply(struct component_steer *steer, float current_pos[2], float *velocity)
{
	// how lovely
	switch(steer->type)
	{
		case STEERING_SEEK:
			handle_seek(current_pos, steer->goal_x, steer->goal_y, velocity);
			break;

		case STEERING_ARRIVE:
			handle_arrive(current_pos, steer->goal_x, steer->goal_y, velocity);
			break;

		default:
			break;
	}
}

static struct steering_path_waypoint *create_node(float pos[2])
{
	struct steering_path_waypoint *waypoint;
	safe_malloc_struct(struct steering_path_waypoint, &waypoint);

	waypoint->next = NULL;
	waypoint->pos[0] = pos[0];
	waypoint->pos[1] = pos[1];

	return waypoint;
};

static void free_node(struct steering_path_waypoint *waypoint)
{
	safe_free(waypoint);
}

void steering_path_add(struct component_steer *steer, float waypoint[2])
{
	struct steering_path_waypoint *wp = create_node(waypoint);

	if (!steer->path_front)
	{
		steer->path_front = wp;
	}
	else if (steer->path_end)
	{
		steer->path_end->next = wp;
	}

	steer->path_end = wp;
}

BOOL steering_path_pop(struct component_steer *steer, float *out)
{
	struct steering_path_waypoint *front = steer->path_front;

	// empty
	if (!front)
		return FALSE;

	out[0] = front->pos[0];
	out[1] = front->pos[1];

	steer->path_front = front->next;

	if (steer->path_front == NULL)
		steer->path_end = NULL;

	free_node(front);

	return TRUE;
}


void steering_path_set(struct component_steer *steer, float waypoints[2], unsigned int n)
{
	// remove old
	float rm[2];
	while (steering_path_pop(steer, rm));

	if (n == 0)
	{
		LOG_WARN("Path length must be at least 0");
		return;
	}

	// first element
	struct steering_path_waypoint *wp = create_node(waypoints);
	steer->path_front = wp;

	// the rest
	for (unsigned int i = 1; i < n; ++i)
	{
		struct steering_path_waypoint *next = create_node(waypoints + (i * 2));
		wp->next = next;
		wp = next;
	}
}
