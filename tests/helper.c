#include "helper.h"
#include "simulator/simulator.h"
#include "entity/entity.h"

#include "util/log.h"

int setup_simulator(void **state)
{
	struct simulator *sim = simulator_create(NULL);
	if (sim == NULL)
		return 1;

	*state = sim;
	return 0;
}

int teardown_simulator(void **state)
{
	simulator_destroy(*(struct simulator **) state);
	return 0;
}

int teardown_remove_all_entities(void **state)
{
	struct simulator *sim = (struct simulator *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	entity_id count = entity_get_count(ctx);
	for (entity_id i = 0; i < count; ++i)
	{
		entity_destroy(ctx, entity_get_first(ctx));
	}

	return 0;
}

