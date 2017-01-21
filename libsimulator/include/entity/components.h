#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "entity/entity.h"
#include "world/world_forward.h"

#define COMPONENT_NONE    (0)
#define COMPONENT_PHYSICS (1 << 0)
#define COMPONENT_HUMAN   (1 << 1)

typedef unsigned int entity_mask;
typedef unsigned int component_type;

struct component_physics
{
	world_body body;
};

struct component_human
{
	human_age age;
	human_gender gender;
};


entity_mask entity_get_component_mask(struct entity_ctx *ctx, entity_id e);

void *entity_add_component(struct entity_ctx *ctx, entity_id e, component_type c);

void entity_remove_component(struct entity_ctx *ctx, entity_id e, component_type c);

void* entity_get_component(struct entity_ctx *ctx, entity_id e, component_type c);

// for the love of god, remember to cast the return value to an array of the
// component type BEFORE iterating through it. see test_entity_component_get
void* entity_get_component_array(struct entity_ctx *ctx, component_type c);

#endif
