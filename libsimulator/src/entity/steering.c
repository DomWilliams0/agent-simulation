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

static double length_sqrd(float *v)
{
	return (v[0] * v[0]) + (v[1] * v[1]);
}

static void scale(float *velocity)
{
	double ls = length_sqrd(velocity);
	double l = sqrt(ls);

	velocity[0] *= HUMAN_ACCELERATION / l;
	velocity[1] *= HUMAN_ACCELERATION / l;
}

void steering_apply(struct component_steer *steer, position current_pos, float *velocity)
{
	// how lovely
	switch(steer->type)
	{
		case STEERING_SEEK:
			velocity[0] = steer->goal_x - current_pos[0];
			velocity[1] = steer->goal_y - current_pos[1];
			scale(velocity);

		case STEERING_ARRIVE:
			// slow down

		default:
			break;
	}
}
