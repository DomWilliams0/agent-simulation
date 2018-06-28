#include <entity/ai/brain_system.h>
#include <entity/ai/action.h>
#include <entity/ai/sensor_system.h>
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
	se_system_tick(&sim->ecs);
	br_system_tick(&sim->ecs);
	st_system_tick(&sim->ecs);

	world_step(sim->world);
}

static ecs_id make_test_entity(struct simulator *sim, cpVect pos)
{
	struct ecs *ecs = &sim->ecs;
	ecs_id e = ecs_new(ecs);

	ECS_COMP(physics) *p;
	ecs_add(ecs, e, physics, p);
	p->body = world_create_entity(sim->world);
	world_set_position(p->body, pos);

	ECS_COMP(human) *h;
	ecs_add(ecs, e, human, h);
	h->age = 25;
	h->gender = GENDER_MALE;

	ECS_COMP(steer) *s;
	ecs_add(ecs, e, steer, s);

	return e;
}

void simulator_populate(struct simulator *sim)
{
	ecs_id mover = make_test_entity(sim, cpv(2, 4));
	ECS_COMP(brain) *b;
	ecs_add(&sim->ecs, mover, brain, b);
	ECS_COMP(sensors) *sense;
	ecs_add(&sim->ecs, mover, sensors, sense);
	ecs_get(&sim->ecs, mover, human)->gender = GENDER_FEMALE;

	struct ac_action action = AC_INIT_FOLLOW;
	ac_init(&action, mover + 4);
//	ac_stack_push(&b->action_stack, &action);

	action = AC_INIT_MOVE_TO;
	ac_init(&action, cpv(2, -2));
	ac_stack_push(&b->action_stack, &action);

	return;

	for (int i = 0; i < 4; ++i)
	{
		ecs_id e = make_test_entity(sim, cpv(i, 3));
		ecs_add(&sim->ecs, e, brain, b);

		action = AC_INIT_FLEE;
		ac_init(&action, mover, 2.0);
		ac_stack_push(&b->action_stack, &action);
	}
}

