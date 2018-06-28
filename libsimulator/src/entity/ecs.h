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
	ECS_COMP(physics) _comps_physics[MAX_ENTITIES];
	ECS_COMP(human) _comps_human[MAX_ENTITIES];
	ECS_COMP(steer) _comps_steer[MAX_ENTITIES];
	ECS_COMP(brain) _comps_brain[MAX_ENTITIES];
	ECS_COMP(sensors) _comps_sensors[MAX_ENTITIES];
})

// id is persistent
ecs_id ecs_new(struct ecs *self);
// TODO destroy entity
bool ecs_is_alive(struct ecs *self, ecs_id e);

// TODO assert has component?
#define ecs_get(ecs, e, type) \
	(ecs_all((ecs), type) + (e))

#define ecs_add(ecs, e, type, out_var) \
do {\
	_ecs_enable((ecs), (e), ECS_COMP_MASK(type)); \
	ECS_COMP(type) *comp = ecs_get(ecs, e, type); \
	ECS_COMP_INIT(type)(comp); \
	(out_var) = comp; \
} while(0)

#define ecs_remove(ecs, e, type) \
do {\
	ECS_COMP_DESTROY(type)(ecs_get((ecs), (e), type)); \
	_ecs_disable((ecs), (e), ECS_COMP_MASK(type)); \
} while(0)

#define ecs_all(ecs, type) \
	((&(ecs)->_comps_ ## type)[0])

#define ecs_has(ecs, e, type) \
	ecs_has_mask(ecs, e, ECS_COMP_MASK(type))

void _ecs_enable(struct ecs *self, ecs_id e, int mask);
void _ecs_disable(struct ecs *self, ecs_id e, int mask);
bool ecs_has_mask(struct ecs *self, ecs_id e, ecs_mask mask);

#endif
