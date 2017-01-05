#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "entity/entity.h"

#define COMPONENT_NONE    (0)
#define COMPONENT_PHYSICS (1 << 0)
#define COMPONENT_HUMAN   (1 << 1)

typedef unsigned int entity_mask;
typedef unsigned int component_type;

struct component_physics
{
	// TODO physics engine
	int x, y;
};

struct component_human
{
	human_age age;
	human_gender gender;
};


entity_mask entity_get_component_mask(struct entity_ctx *ctx, entity_id e);

void entity_add_component(struct entity_ctx *ctx, entity_id e, component_type c);

void entity_remove_component(struct entity_ctx *ctx, entity_id e, component_type c);

#endif
