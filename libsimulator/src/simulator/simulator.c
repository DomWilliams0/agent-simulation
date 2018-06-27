#include <entity/ai/brain_system.h>
#include <entity/ai/action.h>
#include "simulator/simulator.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

MOD_INIT(simulator, {
	random_init();

	// TODO pass through simulator args
	struct world_parameters world_params;
	world_params.width = 30;
	world_params.height = 30;

	if (ecs_init(&self->ecs, NULL) != 0)
		return 1;

	safe_malloc(world_sizeof(), &self->world); // hmmm
	if (world_init(self->world, &world_params) != 0)
		return 2;

	return 0;
});

MOD_DESTROY(simulator, {
	ecs_destroy(&self->ecs);
	if (self->world != NULL)
	{
		world_destroy(self->world);
		safe_free(self->world);
		self->world = NULL;
	}
})

void simulator_step(struct simulator *sim)
{
	br_system_tick(&sim->ecs);
	st_system_tick(&sim->ecs);

	world_step(sim->world);
}

static ecs_id make_test_entity(struct simulator *sim, double pos[2])
{
	struct ecs *ecs = &sim->ecs;
	ecs_id e = ecs_new(ecs);

	ecs_add(ecs, e, ECS_COMP_PHYSICS);
	ecs_add(ecs, e, ECS_COMP_HUMAN);
	ecs_add(ecs, e, ECS_COMP_BRAIN);
	ecs_add(ecs, e, ECS_COMP_STEER);

	struct ecs_comp_physics *p = ecs_get(ecs, e, ECS_COMP_PHYSICS, struct ecs_comp_physics);
	p->body = world_create_entity(sim->world);
	world_set_position(p->body, pos);

	struct ecs_comp_human *h = ecs_get(ecs, e, ECS_COMP_HUMAN, struct ecs_comp_human);
	h->age = 25;
	h->gender = GENDER_MALE;

	struct ecs_comp_brain *b = ecs_get(ecs, e, ECS_COMP_BRAIN, struct ecs_comp_brain);
	struct ac_action default_idle = AC_INIT_IDLE;
	ac_stack_init(&b->action_stack, &default_idle);
	ev_queue_init(&b->event_queue);

	return e;
}

void simulator_populate(struct simulator *sim)
{
	double pos[2] = {2.0, 4.0};
	ecs_id mover = make_test_entity(sim, pos);
	ecs_add(&sim->ecs, mover, ECS_COMP_BRAIN);
	struct ecs_comp_brain *b = ecs_get(&sim->ecs, mover, ECS_COMP_BRAIN, struct ecs_comp_brain);

	struct ac_action action = AC_INIT_MOVE_TO;
	double target[2] = {pos[0], -pos[1]};
	ac_init(&action, target);
	ac_stack_push(&b->action_stack, &action);

	for (int i = 0; i < 4; ++i)
	{
		double pos[2] = {i, 0.0};
		ecs_id e = make_test_entity(sim, pos);
		ecs_add(&sim->ecs, e, ECS_COMP_BRAIN);
		b = ecs_get(&sim->ecs, e, ECS_COMP_BRAIN, struct ecs_comp_brain);

		action = AC_INIT_FLEE;
		ac_init(&action, mover);
		ac_stack_push(&b->action_stack, &action);

	}
}

