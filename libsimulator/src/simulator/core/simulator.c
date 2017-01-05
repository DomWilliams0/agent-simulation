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

void simulator_init(struct simulator_state **sim)
{
	static simulator_id last_id = 1;
	struct simulator_state *new_sim;
	safe_malloc_struct(struct simulator_state, &new_sim);

	new_sim->id = last_id++;
	entity_init_context(&new_sim->entity);
	world_create(&new_sim->world);

	*sim = new_sim;
}

void simulator_step(struct simulator_state *sim)
{
	world_step(sim->world);
}

void simulator_destroy(struct simulator_state **sim)
{
	if (sim)
	{
		LOG_DEBUG("Destroying simulator %d", (*sim)->id);

		entity_destroy_context(&(*sim)->entity);
		world_destroy(&(*sim)->world);

		safe_free(*sim);
		*sim = NULL;
	}
}

simulator_id simulator_get_id(struct simulator_state *sim)
{
	return sim->id;
}
