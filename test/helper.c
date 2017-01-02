#include "helper.h"
#include "simulator/simulator.h"
#include "entity/entity.h"


int setup_simulator(void **state)
{
	struct simulator_state *sim = NULL;
	simulator_init(&sim);
	if (sim == NULL)
		return 1;

	*state = sim;
	return 0;
}

int teardown_simulator(void **state)
{
	simulator_destroy((struct simulator_state **) state);
	return 0;
}

int teardown_remove_all_entities(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	entity_id i = entity_get_iterator(ctx);

	while (entity_is_valid(i))
	{
		// cache next
		entity_id next = entity_get_next(ctx, i);

		// destroy current
		entity_destroy(ctx, i);

		i = next;
	}

	return 0;
}

