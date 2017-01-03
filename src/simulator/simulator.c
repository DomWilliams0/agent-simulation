#include "simulator/simulator.h"
#include "entity/entity.h"
#include "util/memory.h"
#include "util/log.h"

struct simulator_state
{
	simulator_id id;
	struct entity_ctx *entity;
};

void simulator_init(struct simulator_state **sim)
{
	static simulator_id last_id = 1;
	struct simulator_state *new_sim;
	safe_malloc_struct(struct simulator_state, &new_sim);

	new_sim->id = last_id++;
	entity_init_context(&new_sim->entity);

	*sim = new_sim;
}

void simulator_step(struct simulator_state *sim)
{
}

void simulator_destroy(struct simulator_state **sim)
{
	if (sim)
		safe_free(*sim);

	*sim = NULL;
}

simulator_id simulator_get_id(struct simulator_state *sim)
{
	return sim->id;
}

int main(int argc, char **argv)
{
	LOG_INFO("Doing nothing for now");
}
