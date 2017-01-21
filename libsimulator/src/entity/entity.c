#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "entity/entity.h"
#include "entity/components.h"
#include "simulator/simulator.h"
#include "util/log.h"
#include "util/memory.h"
#include "util/util.h"

#define INVALID_ENTITY (MAX_ENTITIES)
#define EMPTY_MASK     (0)
#define MAX_ENTITIES   (256)

struct entity_ctx
{
	entity_id count;

	// current unused
	// uint8_t attributes[MAX_ENTITIES]; // will be bitfield for entity flags

	// these must remain sorted
	// 0 -> count-1 are active
	// count -> MAX_ENTITIES are inactive
	entity_mask masks[MAX_ENTITIES];
	struct component_physics components_physics[MAX_ENTITIES];
	struct component_human components_human[MAX_ENTITIES];
};

// TODO temporary: add module for context lookup when others are added
static struct entity_ctx *context_instance;

struct entity_ctx *entity_create_context()
{
	struct entity_ctx *new_ctx;
	safe_malloc_struct(struct entity_ctx, &new_ctx);

	new_ctx->count = 0;

	context_instance = new_ctx;
	return new_ctx;
}

void entity_destroy_context(struct entity_ctx *ctx)
{
	safe_free(ctx);
}

struct entity_ctx *entity_get_context(struct simulator_state *sim)
{
	// TODO awful but temporary
	if (simulator_get_id(sim) != 1)
		return NULL;

	return context_instance;
}

static BOOL is_valid(entity_id e)
{
	return e < MAX_ENTITIES;
}

/*
static BOOL attr_does_exist(struct entity_ctx *ctx, entity_id e)
{
	return ctx->attributes[e] != 0;
}

static void attr_init(struct entity_ctx *ctx, entity_id e)
{
	ctx->attributes[e] = 1;
}

static void attr_destroy(struct entity_ctx *ctx, entity_id e)
{
	ctx->attributes[e] = 0;
}

static entity_id find_first_valid(struct entity_ctx *ctx, entity_id start_index)
{
	for (int i = start_index; i <= MAX_ENTITIES && ctx->count > 0; ++i)
		if (attr_does_exist(ctx, i))
			return i;

	return INVALID_ENTITY;
}

static entity_id find_first_space(struct entity_ctx *ctx)
{
	// no more
	if (ctx->count == MAX_ENTITIES)
		return INVALID_ENTITY;

	// first place to look
	if (ctx->count > 0 && !ctx->attributes[ctx->count])
		return ctx->count;

	// iterate
	for (int i = 1; i <= MAX_ENTITIES; ++i)
		if (!ctx->attributes[i])
			return i;

	return INVALID_ENTITY;
}
*/

// don't rely on an entity_id being the same in the future
entity_id entity_create(struct entity_ctx *ctx)
{
	// max reached
	if (ctx->count == MAX_ENTITIES)
	{
		LOG_DEBUG("Maximum entity count reached (%d)", MAX_ENTITIES);
		return INVALID_ENTITY;
	}

	// allocate at end of sorted array
	entity_id index = ctx->count;

	LOG_DEBUG("Creating new entity %d", index);

	ctx->count += 1;

	// attr_init(ctx, index);

	return index;
}

void entity_destroy(struct entity_ctx *ctx, entity_id e)
{
	LOG_DEBUG("Destroying entity %d", e);

	ctx->count -= 1;
	entity_id new_count = ctx->count;

	// swap with last active
	SWAP_IN_ARRAY(entity_mask, ctx->masks, e, new_count);
	SWAP_IN_ARRAY(struct component_physics, ctx->components_physics, e, new_count);
	SWAP_IN_ARRAY(struct component_human, ctx->components_human, e, new_count);

	// attr_destroy(ctx, e);
}

BOOL entity_is_alive(struct entity_ctx *ctx, entity_id e)
{
	return is_valid(e) && e < ctx->count;
}

entity_id entity_get_first(struct entity_ctx *ctx)
{
	return ctx->count == 0 ? INVALID_ENTITY : 0;
}

entity_id entity_get_count(struct entity_ctx *ctx)
{
	return ctx->count;
}

entity_id entity_get_max_count(struct entity_ctx *ctx)
{
	UNUSED(ctx);
	return MAX_ENTITIES;
}

void entity_foreach(struct entity_ctx *ctx, entity_consumer *func, void *arg)
{
	for (entity_id e = 0; e < ctx->count; ++e)
		func(e, arg);
}

entity_mask entity_get_component_mask(struct entity_ctx *ctx, entity_id e)
{
	// TODO assert
	if (!entity_is_alive(ctx, e))
		return EMPTY_MASK;

	return ctx->masks[e];
}

static BOOL entity_has_component(struct entity_ctx *ctx, entity_id e, component_type c)
{
	return ctx->masks[e] & c;
}

void* entity_get_component_array(struct entity_ctx *ctx, component_type c)
{
	switch(c)
	{
		case COMPONENT_PHYSICS:
			return ctx->components_physics;

		case COMPONENT_HUMAN:
			return ctx->components_human;

		default:
			// TODO assert false
			LOG_DEBUG("Component not implemented!");
			exit(3);
	}
}

static void* get_component(struct entity_ctx *ctx, entity_id e, component_type c)
{
	switch(c)
	{
		case COMPONENT_PHYSICS:
			return ctx->components_physics + e;

		case COMPONENT_HUMAN:
			return ctx->components_human + e;

		default:
			// TODO assert false, just like above
			LOG_DEBUG("Component not implemented!");
			exit(3);
	}
}

void *entity_add_component(struct entity_ctx *ctx, entity_id e, component_type c)
{
	// TODO assert
	if (entity_is_alive(ctx, e))
		ctx->masks[e] |= c;

	return get_component(ctx, e, c);
}

void entity_remove_component(struct entity_ctx *ctx, entity_id e, component_type c)
{
	if (entity_is_alive(ctx, e) && entity_has_component(ctx, e, c))
		ctx->masks[e] &= ~c;
}

void* entity_get_component(struct entity_ctx *ctx, entity_id e, component_type c)
{
	if (!entity_has_component(ctx, e, c))
		return NULL;

	return get_component(ctx, e, c);
}
