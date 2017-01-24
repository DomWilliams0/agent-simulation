#include <ode/ode.h>

#include "util/memory.h"
#include "util/bool.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#include "world/world.h"

static BOOL ode_init;

struct world
{
	dWorldID phys_id;
	dSpaceID collision_space;
	dJointGroupID contacts;
};

static void create_physics_world(struct world *world)
{
	if (!ode_init)
	{
		ode_init = TRUE;
		dInitODE();
	}

	dWorldID w = dWorldCreate();
	dWorldSetGravity(w, 0, 0, 0);
	dWorldSetQuickStepNumIterations(w, 5);

	dVector3 centre = { 0, 0, 0, 0 };
	dVector3 extents = { 5, 5, 5, 0 };
	dSpaceID s = dQuadTreeSpaceCreate(0, centre, extents, 4);

	world->phys_id = w;
	world->collision_space = s;
	world->contacts = dJointGroupCreate(0);
}

static void destroy_physics_world(struct world *world)
{
	dWorldDestroy(world->phys_id);
	dSpaceDestroy(world->collision_space);
	dJointGroupDestroy(world->contacts);
}

MODULE_IMPLEMENT(struct world, "world",
		world_create,
		{
			create_physics_world(new_instance);
		},
		world_destroy,
		{
			destroy_physics_world(instance);
		})

static void near_callback(void *data, dGeomID o1, dGeomID o2)
{
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnected(b1, b2))
		return;


	struct world *w = (struct world *)data;

	// TODO this absolutely needs tweaking, the current values are meaningless
	dContact contact;
	contact.surface.mode = dContactBounce;
	contact.surface.bounce = 10;
	contact.surface.bounce_vel = 10;
	contact.surface.mu = 0;

	if (dCollide(o1, o2, 1, &contact.geom, sizeof(dContactGeom)))
	{
		dJointID c = dJointCreateContact(w->phys_id, w->contacts, &contact);
		dJointAttach(c, b1, b2);
	}
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
