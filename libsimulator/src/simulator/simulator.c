#include "simulator/simulator.h"
#include "entity/entity.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

struct simulator_state
{
	simulator_id id;
	struct entity_ctx *entity;
	struct world *world;
};

struct simulator_state *simulator_create()
{
	static simulator_id last_id = 1;
	struct simulator_state *new_sim;
	safe_malloc_struct(struct simulator_state, &new_sim);

	new_sim->id = last_id++;
	LOG_DEBUG("Creating new simulator %d", new_sim->id);

	if ((new_sim->entity = entity_create_context()) == NULL ||
		(new_sim->world = world_create()) == NULL)
	{
		safe_free(new_sim);
		new_sim = NULL;
	}

	return new_sim;
}

void simulator_step(struct simulator_state *sim)
{
	world_step(sim->world);
}

void simulator_destroy(struct simulator_state *sim)
{
	if (sim)
	{
		LOG_DEBUG("Destroying simulator %d", sim->id);

		entity_destroy_context(sim->entity);
		world_destroy(sim->world);

		safe_free(sim);
	}
}

simulator_id simulator_get_id(struct simulator_state *sim)
{
	return sim->id;
}

struct world *simulator_get_world(struct simulator_state *sim)
{
	return sim->world;
}
