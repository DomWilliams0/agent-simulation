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
	st_system_tick(&sim->ecs);
	world_step(sim->world);
}

static void make_test_entity(struct simulator *sim, double pos[2])
{
	struct ecs *ecs = &sim->ecs;
	ecs_id e = ecs_new(ecs);

	ecs_add(ecs, e, ECS_COMP_PHYSICS);
	ecs_add(ecs, e, ECS_COMP_HUMAN);
	ecs_add(ecs, e, ECS_COMP_STEER);

	struct ecs_comp_physics *p = ecs_get(ecs, e, ECS_COMP_PHYSICS, struct ecs_comp_physics);
	p->body = world_create_entity(sim->world);
	world_set_position(p->body, pos);

	struct ecs_comp_human *h = ecs_get(ecs, e, ECS_COMP_HUMAN, struct ecs_comp_human);
	h->age = 25;
	h->gender = GENDER_MALE;

	struct ecs_comp_steer *s = ecs_get(ecs, e, ECS_COMP_STEER, struct ecs_comp_steer);
	s->type = ST_ARRIVE;
	s->target[0] = 0.0;
	s->target[1] = 0.0;
	s->separation = true;
}

void simulator_populate(struct simulator *sim)
{
	for (int i = 0; i < 4; ++i)
	{
		double pos[2] = {-i, i};
		make_test_entity(sim, pos);
	}
}

