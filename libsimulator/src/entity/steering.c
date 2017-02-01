#include <ode/ode.h>
#include <math.h>

#include "entity/steering.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/log.h"

void steering_update_system(struct entity_ctx *entities, struct world *world)
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

static inline void handle_seek(position pos, float goal_x, float goal_y, float *velocity)
{
	// full speed ahead
	velocity[0] = goal_x - pos[0];
	velocity[1] = goal_y - pos[1];
	scale(velocity, HUMAN_ACCELERATION);
}

static inline void handle_arrive(position pos, float goal_x, float goal_y, float *velocity)
{
	velocity[0] = goal_x - pos[0];
	velocity[1] = goal_y - pos[1];

	double distance = length(velocity);
	float speed = HUMAN_ACCELERATION;
	if (distance < STEERING_ARRIVE_RADIUS)
		speed *= distance / STEERING_ARRIVE_RADIUS;

	scale(velocity, speed);
}

void steering_apply(struct component_steer *steer, position current_pos, float *velocity)
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
