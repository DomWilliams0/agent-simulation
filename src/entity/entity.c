#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "simulator/simulator.h"
#include "entity/entity.h"
#include "util/log.h"

#define INVALID_ENTITY (0)
#define MAX_ENTITIES (256)

struct entity_ctx
{
	entity_id count;
	uint8_t attributes[MAX_ENTITIES + 1]; // will be bitfield for entity flags
};

// TODO temporary: add module for context lookup when others are added
static struct entity_ctx *context_instance;

void entity_init_context(struct entity_ctx **ctx)
{
	struct entity_ctx *new_ctx = malloc(sizeof(struct entity_ctx));
	// TODO null check and use own allocator

	new_ctx->count = 0;

	*ctx = new_ctx;
	context_instance = new_ctx;
}

void entity_destroy_context(struct entity_ctx **ctx)
{
	// TODO generalise and share with simulator_state
	if (ctx && *ctx)
		free(*ctx);

	*ctx = NULL;
}

struct entity_ctx *entity_get_context(struct simulator_state *sim)
{
	// TODO awful but temporary
	if (simulator_get_id(sim) != 1)
		return NULL;

	return context_instance;
}

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

entity_id entity_create(struct entity_ctx *ctx)
{
	entity_id index = find_first_space(ctx);
	if (index == INVALID_ENTITY)
	{
		LOG_DEBUG("Could not create a new entity");
		return INVALID_ENTITY;
	}

	ctx->count += 1;

	attr_init(ctx, index);

	return index;
}

void entity_destroy(struct entity_ctx *ctx, entity_id e)
{
	if (entity_is_valid(e) && attr_does_exist(ctx, e))
	{
		attr_destroy(ctx, e);
		ctx->count -= 1;
	}
}

BOOL entity_is_valid(entity_id e)
{
	return e != INVALID_ENTITY && e <= MAX_ENTITIES;
}

entity_id entity_get_count(struct entity_ctx *ctx)
{
	return ctx->count;
}

entity_id entity_get_max_count(struct entity_ctx *ctx)
{
	return MAX_ENTITIES;
}

entity_id entity_get_iterator(struct entity_ctx *ctx)
{
	return find_first_valid(ctx, 0);
}

entity_id entity_get_next(struct entity_ctx *ctx, entity_id e)
{
	return find_first_valid(ctx, e + 1);
}
