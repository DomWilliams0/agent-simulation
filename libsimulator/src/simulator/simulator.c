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
			world_params.chunk_width = 5;
			world_params.chunk_height = 5;
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
	struct position pos = {40, 40};

	for (int i = 0; i < 10; ++i)
	{
		entity_id e = entity_create(entity);

		struct component_physics *phys = entity_add_component(entity, e, COMPONENT_PHYSICS);
		struct component_human *hum = entity_add_component(entity, e, COMPONENT_HUMAN); // cache miss, hissss

		phys->body = world_create_entity(sim->world);
		world_set_position(phys->body, &pos);
		pos.x += 5;
		pos.y += 5;

		hum->age = 20 + i;
		hum->gender = i % 2 == 0 ? MALE : FEMALE;
	}
}

struct entity_ctx *entity_get_context(struct simulator *sim)
{
	return sim->entity;
}

struct world *world_get_world(struct simulator *sim)
{
	return sim->world;
}

