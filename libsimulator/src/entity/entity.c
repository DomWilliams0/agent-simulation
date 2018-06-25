#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "entity/entity.h"
#include "util/log.h"
#include "util/memory.h"

#define EMPTY_MASK     (0)
#define INVALID_ENTITY (MAX_ENTITIES)

MOD_INIT(entities, {
	self->count = 0;
	return 0;
})

MOD_DESTROY(entities, {})

static bool is_valid(entity_id e)
{
	return e < MAX_ENTITIES;
}

/*
static bool attr_does_exist(struct entities *self, entity_id e)
{
	return self->attributes[e] != 0;
}

static void attr_init(struct entities *self, entity_id e)
{
	self->attributes[e] = 1;
}

static void attr_destroy(struct entities *self, entity_id e)
{
	self->attributes[e] = 0;
}

static entity_id find_first_valid(struct entities *self, entity_id start_index)
{
	for (int i = start_index; i <= MAX_ENTITIES && self->count > 0; ++i)
		if (attr_does_exist(self, i))
			return i;

	return INVALID_ENTITY;
}

static entity_id find_first_space(struct entities *self)
{
	// no more
	if (self->count == MAX_ENTITIES)
		return INVALID_ENTITY;

	// first place to look
	if (self->count > 0 && !self->attributes[self->count])
		return self->count;

	// iterate
	for (int i = 1; i <= MAX_ENTITIES; ++i)
		if (!self->attributes[i])
			return i;

	return INVALID_ENTITY;
}
*/

// don't rely on an entity_id being the same in the future
entity_id entity_create(struct entities *self)
{
	// max reached
	if (self->count == MAX_ENTITIES)
	{
		LOG_DEBUG("Maximum entity count reached (%d)", MAX_ENTITIES);
		return INVALID_ENTITY;
	}

	// allocate at end of sorted array
	entity_id index = self->count;

	LOG_DEBUG("Creating new entity %d", index);

	self->count += 1;

	// attr_init(self, index);

	return index;
}

void entity_destroy(struct entities *self, entity_id e)
{
	LOG_DEBUG("Destroying entity %d", e);

	self->count -= 1;
	entity_id new_count = self->count;

	// swap with last active
	// TODO what if doesnt have all components?
	SWAP_IN_ARRAY(entity_mask, self->masks, e, new_count);
	SWAP_IN_ARRAY(struct component_physics, self->components_physics, e, new_count);
	SWAP_IN_ARRAY(struct component_human, self->components_human, e, new_count);
	SWAP_IN_ARRAY(struct component_steer, self->components_steer, e, new_count);

	// attr_destroy(self, e);
}

bool entity_is_alive(struct entities *self, entity_id e)
{
	return is_valid(e) && e < self->count;
}

entity_id entity_get_first(struct entities *self)
{
	return self->count == 0 ? INVALID_ENTITY : 0;
}

entity_id entity_get_max_count(struct entities *self)
{
	UNUSED(self);
	return MAX_ENTITIES;
}

void entity_foreach(struct entities *self, entity_consumer *func, void *arg)
{
	for (entity_id e = 0; e < self->count; ++e)
		func(e, arg);
}

entity_mask entity_get_component_mask(struct entities *self, entity_id e)
{
	// TODO assert
	if (!entity_is_alive(self, e))
		return EMPTY_MASK;

	return self->masks[e];
}

void* entity_get_component_array(struct entities *self, enum component_type c)
{
	switch(c)
	{
		case COMPONENT_PHYSICS:
			return self->components_physics;

		case COMPONENT_HUMAN:
			return self->components_human;

		case COMPONENT_STEER:
			return self->components_steer;

		default:
			// TODO assert false
			LOG_ERROR("Component not implemented!");
			LOG_FLUSH;
			exit(3);
	}
}

bool entity_has_component(struct entities *self, entity_id e, entity_mask mask)
{
	if (entity_is_alive(self, e))
		return (self->masks[e] & mask) == mask;

	return false;
}

static void* get_component(struct entities *self, entity_id e, enum component_type c)
{
	switch(c)
	{
		case COMPONENT_PHYSICS:
			return self->components_physics + e;

		case COMPONENT_HUMAN:
			return self->components_human + e;

		case COMPONENT_STEER:
			return self->components_steer + e;

		default:
			// TODO assert false, just like above
			LOG_ERROR("Component not implemented!");
			LOG_FLUSH;
			exit(3);
	}
}

void *entity_add_component(struct entities *self, entity_id e, enum component_type c)
{
	// TODO assert
	if (entity_is_alive(self, e))
		self->masks[e] |= c;

	return get_component(self, e, c);
}

void entity_remove_component(struct entities *self, entity_id e, enum component_type c)
{
	if (entity_has_component(self, e, c))
		self->masks[e] &= ~c;
}

void* entity_get_component(struct entities *self, entity_id e, enum component_type c)
{
	if (!entity_has_component(self, e, c))
		return NULL;

	return get_component(self, e, c);
}
