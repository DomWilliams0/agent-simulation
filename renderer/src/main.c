#define _GNU_SOURCE
#include <unistd.h>

#include "simulator/simulator.h"
#include "entity/entity.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/log.h"
#include "util/bool.h"

#define TICKS_PER_SECOND (5)

void do_debug_things(struct simulator_state *sim)
{
	struct entity_ctx *ctx = entity_get_context(sim);

	// print all
	entity_id it = entity_get_iterator(ctx);
	struct position pos;

	while (entity_is_alive(ctx, it))
	{
		struct component_physics *phys = entity_get_component(ctx, it, COMPONENT_PHYSICS);
		world_get_position(phys->body, &pos);
		LOG_INFO("Entity %d at (%lf, %lf)", it, pos.x, pos.y);

		it = entity_get_next(ctx, it);
	}
}

static void create_test_entity(struct simulator_state *sim, coord x, coord y)
{
	struct entity_ctx *ctx = entity_get_context(sim);

	entity_id e = entity_create(ctx);
	if (!entity_is_alive(ctx, e))
		return;

	entity_add_component(ctx, e, COMPONENT_PHYSICS);
	struct component_physics *p = entity_get_component(ctx, e, COMPONENT_PHYSICS);
	p->body = world_create_entity(simulator_get_world(sim));

	struct position pos = {x, y};
	world_set_position(p->body, &pos);
}

void init(struct simulator_state **sim)
{
	simulator_init(sim);
	LOG_INFO("Created simulator");

	for (int i = 0; i < 3; ++i)
		create_test_entity(*sim, i, i + 10);
}

void loop(struct simulator_state *sim)
{
	double sleep_time = (1.0 / TICKS_PER_SECOND) * 1000 * 1000;
	BOOL running = TRUE;
	while (running)
	{
		simulator_step(sim);
		LOG_INFO("Stepping...");

		do_debug_things(sim);

		usleep(sleep_time);
	}


}
void stop(struct simulator_state **sim)
{
	simulator_destroy(sim);
	LOG_INFO("Destroyed simulator");
}

int main()
{
	struct simulator_state *sim;
	init(&sim);

	loop(sim);

	stop(&sim);
	return 0;
}

