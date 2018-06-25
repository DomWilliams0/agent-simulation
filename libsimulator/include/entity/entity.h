#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include "util/util.h"
#include "entity/components.h"

#define MAX_ENTITIES   (256)

struct simulator;

MOD_DECLARE(entities, {
	entity_id count;

	/* currently unused: will be bitfield for entity flags */
	/* uint8_t attributes[MAX_ENTITIES]; */

	/* these must remain sorted. 0 -> count-1 are active, count -> MAX_ENTITIES are inactive */
	entity_mask masks[MAX_ENTITIES];
	struct component_physics components_physics[MAX_ENTITIES];
	struct component_human components_human[MAX_ENTITIES];
	struct component_steer components_steer[MAX_ENTITIES];
})

struct entities *entity_get_context(struct simulator *sim);

entity_id entity_create(struct entities *self);

void entity_destroy(struct entities *self, entity_id e);

bool entity_is_alive(struct entities *self, entity_id e);

entity_id entity_get_first(struct entities *self);

entity_id entity_get_count(struct entities *self);

entity_id entity_get_max_count(struct entities *self);

void entity_foreach(struct entities *self, entity_consumer *func, void *arg);

#endif
