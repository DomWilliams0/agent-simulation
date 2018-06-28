#include <math.h>

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

#include "world/world.h"
#include "entity/ecs.h"

static bool load_params(struct world *world, struct world_parameters *params);

MOD_INIT(world, {
	vec_init(&self->roads);

	struct world_parameters *params = (struct world_parameters *)arg;
	if (!load_params(self, params))
	{
		LOG_WARN("Invalid world parameters");
		return 1;
	}

	// collision space
	cpSpace *space = cpSpaceNew();
	cpSpaceSetGravity(space, cpv(0.0, 0.0));
	cpSpaceSetIterations(space, 10);
	cpSpaceSetDamping(space, WORLD_DAMPING);
	self->space = space;
	// TODO add world boundary

	// test segment
	struct road test_road;
	test_road.a = cpv(0, 1.2);
	test_road.b = cpv(4, 3.5);
	test_road.thickness = 0.2;
	world_add_road(self, &test_road);

	return 0;
})

MOD_DESTROY(world, {
	if (self->space)
	{
		cpSpaceFree(self->space);
		self->space = NULL;
	}
})

void world_step(struct world *w)
{
	cpSpaceStep(w->space, 1/20.f);
}

world_body world_create_entity(struct world *w)
{
	// body
	cpBody *b = cpSpaceAddBody(w->space, cpBodyNew(HUMAN_MASS, INFINITY));

	// collision
	cpShape *shape = cpSpaceAddShape(w->space, cpCircleShapeNew(b, HUMAN_RADIUS, cpvzero));
	cpShapeSetFriction(shape, HUMAN_FRICTION);

	return b;
}

cpVect world_get_position(world_body body)
{
	return cpBodyGetPosition(body);
}

void world_set_position(world_body body, cpVect pos)
{
	cpBodySetPosition(body, pos);
}

cpVect world_get_velocity(world_body body)
{
	return cpBodyGetVelocity(body);
}

static bool load_params(struct world *world, struct world_parameters *params)
{
	if (params == NULL)
	{
		LOG_WARN("No parameters provided");
		return false;
	}

	if (params->width <= 0 || params->height <= 0)
	{
		LOG_WARN("Width and height must be positive");
		return false;
	}

	world->width = params->width;
	world->height = params->height;

	return true;
}
