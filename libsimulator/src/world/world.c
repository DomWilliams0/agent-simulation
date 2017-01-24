#include "util/memory.h"
#include "util/bool.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#include "world/world.h"
#include "world/internal/physics.h"
#include "world/internal/world.h"

static BOOL load_params(struct world *world, struct world_parameters *params)
{
	if (params == NULL)
	{
		LOG_WARN("No parameters provided");
		return FALSE;
	}

	if (params->width <= 0 || params->height <= 0)
	{
		LOG_WARN("Invalid width or height");
		return FALSE;
	}

	// currently unsupported
	if (params->file_path != NULL)
	{
		LOG_WARN("World loading currently not supported");
		return FALSE;
	}

	world->width = params->width;
	world->height = params->height;
	world->file_path = params->file_path;

	return TRUE;
}

MODULE_IMPLEMENT(struct world, "world",
		world_create,
		{
			struct world_parameters *params = (struct world_parameters *)arg;
			if (!load_params(new_instance, params))
			{
				LOG_WARN("Invalid world parameters");
				MODULE_INIT_ABORT;
			}

			create_physics_world(new_instance);
		},
		world_destroy,
		{
			destroy_physics_world(instance);
		})

int world_get_width(struct world *w)
{
	return w->width;
}

int world_get_height(struct world *w)
{
	return w->height;
}

char *world_get_file_path(struct world *w)
{
	return w->file_path;
}

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

	// collision
	dGeomID geom = dCreateSphere(w->collision_space, HUMAN_RADIUS);
	dGeomSetBody(geom, b);

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
