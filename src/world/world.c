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

void world_create(struct world **w)
{
	struct world *new_world;
	safe_malloc_struct(struct world, &new_world);

	new_world->phys_id = create_physics_world();

	*w = new_world;

	LOG_DEBUG("Created new world");
}

void world_destroy(struct world **w)
{
	if (w)
	{
		dWorldDestroy((*w)->phys_id);

		safe_free(*w);
		*w = NULL;

		LOG_DEBUG("Destroyed world");
	}

}

void world_step(struct world *w)
{
	dWorldQuickStep(w->phys_id, 0.05);
}

void world_create_entity(struct world *w, world_body *body)
{
	dBodyID b = dBodyCreate(w->phys_id);

	dMass mass;
	dMassSetCapsuleTotal(&mass, 80, 3, 0.3, 1.7);
	dBodySetMass(b, &mass);

	*body = b;
}
