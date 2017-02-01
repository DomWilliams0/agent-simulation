#include "simulator/simulator.h"
#include "entity/entity.h"
#include "entity/components.h"
#include "world/world.h"

#include "util/memory.h"
#include "util/log.h"

struct simulator
{
	simulator_id id;
	struct entity_ctx *entity;
	struct world *world;
};

MODULE_IMPLEMENT(struct simulator, "simulator",
		simulator_create,
		{
			static simulator_id last_id = 1;
			new_instance->id = last_id++;

			// TODO pass through simulator args
			struct world_parameters world_params;
			world_params.chunk_width = 4;
			world_params.chunk_height = 4;
			world_params.file_path = NULL;

			if ((new_instance->entity = entity_create_context(NULL)) == NULL ||
				(new_instance->world = world_create(&world_params)) == NULL)
			{
				MODULE_INIT_ABORT;
			}
		},
		simulator_destroy,
		{
			if (instance->entity)
				entity_destroy_context(instance->entity);
			if (instance->world)
				world_destroy(instance->world);
		})

void simulator_step(struct simulator *sim)
{
	steering_update_system(sim->entity);
	world_step(sim->world);
}

simulator_id simulator_get_id(struct simulator *sim)
{
	return sim->id;
}

struct world *simulator_get_world(struct simulator *sim)
{
	return sim->world;
}

void simulator_populate(struct simulator *sim)
{
	struct entity_ctx *entity = sim->entity;
	float pos[2] = {0, 0};
	entity_id e = entity_create(entity);

	struct component_physics *phys = entity_add_component(entity, e, COMPONENT_PHYSICS);
	phys->body = world_create_entity(sim->world);
	world_set_position(phys->body, pos);

	struct component_human *hum = entity_add_component(entity, e, COMPONENT_HUMAN);
	hum->age = 20;
	hum->gender = MALE;

	struct component_steer *steer = entity_add_component(entity, e, COMPONENT_STEER);
	steer->type = STEERING_PATH_FOLLOW;

	// silly path
	float path[4][2] = {{5, 0}, {5, 5}, {0, 5}, {5, 5}};
	steering_path_set(steer, (float *)path, 4);

	// some fun terrain
	struct world *world = sim->world;
	for (int i = 0; i < 10; ++i)
		world_set_tile(world, i, i, TILE_GRASS);

}

struct entity_ctx *entity_get_context(struct simulator *sim)
{
	return sim->entity;
}

struct world *world_get_world(struct simulator *sim)
{
	return sim->world;
}

