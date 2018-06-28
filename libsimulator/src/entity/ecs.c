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
	}

	// allocate at end of sorted array
	return self->count++;
}

bool ecs_is_alive(struct ecs *self, ecs_id e)
{
	return e < self->count;
}

void _ecs_enable(struct ecs *self, ecs_id e, int mask)
{
	assert(ecs_is_alive(self, e));
	self->_masks[e] |= mask;
}

void _ecs_disable(struct ecs *self, ecs_id e, int mask)
{
	assert(ecs_is_alive(self, e));
	self->_masks[e] &= ~mask;
}

bool ecs_has_mask(struct ecs *self, ecs_id e, ecs_mask mask)
{
	assert(ecs_is_alive(self, e));
	return (self->_masks[e] & mask) == mask;
}

