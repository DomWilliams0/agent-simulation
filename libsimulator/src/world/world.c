#include <math.h>

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

#include "world/world.h"
#include "world/internal/physics.h"
#include "world/internal/world.h"
#include "entity/steering.h"
#include "entity/entity.h"

static bool load_params(struct world *world, struct world_parameters *params);

MOD_INIT(world, {
	struct world_parameters *params = (struct world_parameters *)arg;
	if (!load_params(self, params))
	{
		LOG_WARN("Invalid world parameters");
		return 1;
	}
	create_physics_world(self);
	return 0;
})

MOD_DESTROY(world, {
	destroy_physics_world(self);
})

void world_step(struct world *w)
{
	dSpaceCollide(w->collision_space, w, near_callback);
	dWorldQuickStep(w->phys_id, 0.05);

	dJointGroupEmpty(w->contacts);
}

world_body world_create_entity(struct world *w)
{
	// body
	dBodyID b = dBodyCreate(w->phys_id);
	dMass mass;
	dMassSetSphereTotal(&mass, HUMAN_MASS, HUMAN_RADIUS);
	dBodySetMass(b, &mass);
	dBodySetLinearDamping(b, HUMAN_DAMPING);

	// collision
	dGeomID geom = dCreateSphere(w->collision_space, HUMAN_RADIUS);
	dGeomSetBody(geom, b);

	return b;
}

void world_get_position(world_body body, double pos[2])
{
	const double *dpos = dBodyGetPosition(body);
	pos[0] = dpos[0];
	pos[1] = dpos[1];
}

void world_set_position(world_body body, double pos[2])
{
	dBodySetPosition(body, pos[0] + 0.5, pos[1] + 0.5, 0);
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
