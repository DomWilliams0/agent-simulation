#ifndef SIM_ENTITY_H
#define SIM_ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include "util/util.h"
#include "entity/components.h"

#define MAX_ENTITIES (256)

typedef uint16_t ecs_mask;

MOD_DECLARE(ecs, {
	ecs_id count;

	/* these must remain sorted. 0 -> count-1 are active, count -> MAX_ENTITIES are inactive */
	ecs_mask _masks[MAX_ENTITIES];
	struct ecs_comp_physics _comps_physics[MAX_ENTITIES];
	struct ecs_comp_human _comps_human[MAX_ENTITIES];
	struct ecs_comp_steer _comps_steer[MAX_ENTITIES];
	struct ecs_comp_brain _comps_brain[MAX_ENTITIES];
})

// id is persistent
ecs_id ecs_new(struct ecs *self);

// TODO destroy entity

bool ecs_is_alive(struct ecs *self, ecs_id e);

void ecs_add(struct ecs *self, ecs_id e, enum ecs_component c);

void ecs_remove(struct ecs *self, ecs_id e, enum ecs_component c);

//void *ecs_get(struct ecs *self, ecs_id e, enum ecs_component c);
#define ecs_get(self, e, comp, comp_type) \
	((comp_type *) ecs_all(self,  comp)) + e

// make sure to cast the return value to the concrete time before dereferencing
void *ecs_all(struct ecs *self, enum ecs_component c);

bool ecs_has(struct ecs *self, ecs_id e, ecs_mask mask);

#endif
