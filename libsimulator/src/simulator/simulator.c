#include "simulator/simulator.h"
#include "entity/entity.h"
#include "entity/components.h"

#include "util/memory.h"
#include "util/log.h"

MOD_INIT(simulator, {
	random_init();

	// TODO pass through simulator args
	struct world_parameters world_params;
	world_params.width = 30;
	world_params.height = 30;

	if (entities_init(&self->entities, NULL) != 0)
		return 1;

	safe_malloc(world_sizeof(), &self->world); // hmmm
	if (world_init(self->world, &world_params) != 0)
		return 2;

	return 0;
})

MOD_DESTROY(simulator, {
	entities_destroy(&self->entities);
	if (self->world != NULL)
		world_destroy(self->world);
})

void simulator_step(struct simulator *sim)
{
	steering_update_system(&sim->entities);
	world_step(sim->world);
}

void simulator_populate(struct simulator *sim)
{
	struct entities *entities = &sim->entities;
	double pos[2] = {0, 0};
	entity_id e = entity_create(entities);

	struct component_physics *phys = entity_add_component(entities, e, COMPONENT_PHYSICS);
	phys->body = world_create_entity(sim->world);
	world_set_position(phys->body, pos);

	struct component_human *hum = entity_add_component(entities, e, COMPONENT_HUMAN);
	hum->age = 20;
	hum->gender = MALE;

	struct component_steer *steer = entity_add_component(entities, e, COMPONENT_STEER);
	steer->type = STEERING_PATH_FOLLOW;

	// silly path
	double path[4][2] = {{5, 0}, {5, 5}, {0, 5}, {0, 0}};
	steering_path_set(steer, (double *)path, 4);
}

