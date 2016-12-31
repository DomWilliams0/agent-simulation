#ifndef ENTITY_H
#define ENTITY_H

#include "util/bool.h"

// entity IDs are unique across all worlds
typedef unsigned int entity_id;

struct simulator_state;

entity_id entity_create(struct simulator_state *sim);

void entity_destroy(entity_id e);

BOOL entity_is_valid(entity_id e);

entity_id entity_get_count(struct simulator_state *sim);

entity_id entity_get_max_count(struct simulator_state *sim);

entity_id entity_get_iterator(struct simulator_state *sim);

entity_id entity_get_next(entity_id e);

#endif
