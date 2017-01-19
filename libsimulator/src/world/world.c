#include <ode/ode.h>

#include "util/memory.h"
#include "util/bool.h"
#include "util/log.h"

#include "world/world.h"

static BOOL ode_init;

struct world
{
	dWorldID phys_id;
};

static dWorldID create_physics_world()
{
	if (!ode_init)
	{
		ode_init = TRUE;
		dInitODE();
	}

	dWorldID w = dWorldCreate();
	dWorldSetGravity(w, 0, 0, 0);
	dWorldSetQuickStepNumIterations(w, 5);

	return w;
}

struct world *world_create()
{
	struct world *new_world;
	safe_malloc_struct(struct world, &new_world);

	new_world->phys_id = create_physics_world();

	LOG_DEBUG("Created new world");

	return new_world;
}

void world_destroy(struct world *w)
{
	if (w)
	{
		dWorldDestroy(w->phys_id);
		safe_free(w);

		LOG_DEBUG("Destroyed world");
	}

}

void world_step(struct world *w)
{
	dWorldQuickStep(w->phys_id, 0.05);
}

 world_body world_create_entity(struct world *w)
{
	dBodyID b = dBodyCreate(w->phys_id);

	dMass mass;
	dMassSetCapsuleTotal(&mass, 80, 3, 0.3, 1.7);
	dBodySetMass(b, &mass);

	return b;
}

void world_get_position(world_body body, struct position *pos)
{
	const dReal *position = dBodyGetPosition(body);
	pos->x = position[0];
	pos->y = position[1];
}

void world_set_position(world_body body, struct position *pos)
{
	dBodySetPosition(body, pos->x, pos->y, 0);
}
