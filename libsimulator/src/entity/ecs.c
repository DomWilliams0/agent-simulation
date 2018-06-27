#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "entity/ecs.h"
#include "util/log.h"

MOD_INIT(ecs, {
	self->count = 0;
	memset(self->_masks, 0, MAX_ENTITIES * sizeof(ecs_mask));
	return 0;
})

MOD_DESTROY(ecs, {})

ecs_id ecs_new(struct ecs *self)
{
	// max reached
	if (self->count == MAX_ENTITIES)
	{
		LOG_ERROR("Maximum entity count reached (%d)", MAX_ENTITIES);
		LOG_FLUSH;
		exit(4); // hmmm
		return 0;
	}

	// allocate at end of sorted array
	return self->count++;
}

bool ecs_is_alive(struct ecs *self, ecs_id e)
{
	return e < self->count;
}

void ecs_add(struct ecs *self, ecs_id e, enum ecs_component c)
{
	assert(ecs_is_alive(self, e));
	self->_masks[e] |= c;
}

void ecs_remove(struct ecs *self, ecs_id e, enum ecs_component c)
{
	assert(ecs_is_alive(self, e));
	self->_masks[e] &= ~c;
}

/*void *ecs_get(struct ecs *self, ecs_id e, enum ecs_component c)
{
	assert(ecs_is_alive(self, e));
	return &ecs_all(self, c)[e];
}*/

void *ecs_all(struct ecs *self, enum ecs_component c)
{
	switch (c)
	{
		case ECS_COMP_PHYSICS:
			return self->_comps_physics;
		case ECS_COMP_HUMAN:;
			return self->_comps_human;
		case ECS_COMP_STEER:
			return self->_comps_steer;
	}

	LOG_ERROR("Component %d not implemented", c);
	LOG_FLUSH;
	exit(5);
	return NULL; // unreachable
}

bool ecs_has(struct ecs *self, ecs_id e, ecs_mask mask)
{
	return (self->_masks[e] & mask) == mask;
}

