#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "entity/entity.h"
#include "entity/steering.h"
#include "world/world_forward.h"

enum component_type {
	COMPONENT_NONE    = (0),
	COMPONENT_PHYSICS = (1 << 0),
	COMPONENT_HUMAN   = (1 << 1),
	COMPONENT_STEER   = (1 << 2),
};

typedef uint16_t entity_mask;

struct component_physics
{
	world_body body;
};

struct component_human
{
	human_age age;
	human_gender gender;
};

struct component_steer
{
	enum steering_type type;
	union
	{
		struct
		{
			int goal_x;
			int goal_y;
		};

		struct
		{
			// pop off from the front
			struct steering_path_waypoint *path_front;

			// add to the end
			struct steering_path_waypoint *path_end;
		};
	};
};


entity_mask entity_get_component_mask(struct entity_ctx *ctx, entity_id e);

void *entity_add_component(struct entity_ctx *ctx, entity_id e, enum component_type c);

void entity_remove_component(struct entity_ctx *ctx, entity_id e, enum component_type c);

void* entity_get_component(struct entity_ctx *ctx, entity_id e, enum component_type c);

// for the love of god, remember to cast the return value to an array of the
// component type BEFORE iterating through it. see test_entity_component_get
void* entity_get_component_array(struct entity_ctx *ctx, enum component_type c);

BOOL entity_has_component(struct entity_ctx *ctx, entity_id e, entity_mask mask);

#endif
