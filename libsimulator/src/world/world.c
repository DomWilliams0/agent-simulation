#include <math.h>

#include "util/memory.h"
#include "util/bool.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#include "world/world.h"
#include "world/internal/physics.h"
#include "world/internal/world.h"

static void load_terrain(struct world *world);
static BOOL load_params(struct world *world, struct world_parameters *params);

MODULE_IMPLEMENT(struct world, "world",
		world_create,
		{
			static unsigned int last_id = 0;

			struct world_parameters *params = (struct world_parameters *)arg;
			if (!load_params(new_instance, params))
			{
				LOG_WARN("Invalid world parameters");
				MODULE_INIT_ABORT;
			}
			new_instance->id = last_id++;
			create_physics_world(new_instance);
			load_terrain(new_instance);
		},
		world_destroy,
		{
			destroy_physics_world(instance);
			if (instance->chunks)
				safe_free(instance->chunks);
		})

unsigned int world_get_width(struct world *w)
{
	return w->width;
}

unsigned int world_get_height(struct world *w)
{
	return w->height;
}

char *world_get_file_path(struct world *w)
{
	return w->file_path;
}

unsigned int world_get_id(struct world *w)
{
	return w->id;
}

void world_step(struct world *w)
{
	dSpaceCollide(w->collision_space, w, near_callback);
	dWorldQuickStep(w->phys_id, 0.05);

	dJointGroupEmpty(w->contacts);
}

world_body world_create_entity(struct world *w)
{
	// body
	dBodyID b = dBodyCreate(w->phys_id);
	dMass mass;
	dMassSetSphereTotal(&mass, HUMAN_MASS, HUMAN_RADIUS);
	dBodySetMass(b, &mass);

	// collision
	dGeomID geom = dCreateSphere(w->collision_space, HUMAN_RADIUS);
	dGeomSetBody(geom, b);

	return b;
}

void world_get_position(world_body body, struct position *pos)
{
	const dReal *position = dBodyGetPosition(body);
	pos->x = position[0];
	pos->y = position[1];
}

void world_set_position(world_body body, struct position *pos)
{
	dBodySetPosition(body, pos->x, pos->y, 0);
}

static BOOL load_params(struct world *world, struct world_parameters *params)
{
	if (params == NULL)
	{
		LOG_WARN("No parameters provided");
		return FALSE;
	}

	if (params->width <= 0 || params->height <= 0)
	{
		LOG_WARN("Width and height must be positive");
		return FALSE;
	}

	if (params->width % CHUNK_SIZE != 0 || params->height % CHUNK_SIZE != 0)
	{
		LOG_WARN("Width and height must be multiples of CHUNK_SIZE");
		return FALSE;
	}

	world->width = params->width;
	world->height = params->height;
	world->file_path = params->file_path;

	return TRUE;
}

static void load_terrain(struct world *world)
{
	unsigned int chunks_hor = ceilf(world->width / (float)CHUNK_SIZE);
	unsigned int chunks_ver = ceilf(world->height / (float)CHUNK_SIZE);

	safe_malloc(chunks_hor * chunks_ver * sizeof(world->chunks[0]), &world->chunks);
}

// #define GET_CHUNK_UNSAFE(w, x, y) \
// 	struct chunk *chunk = w->chunks + ((x / CHUNK_SIZE) + (w->width * (y / CHUNK_SIZE)))

#define GET_CHUNK(w, x, y) \
	unsigned int chunk_x  = x / CHUNK_SIZE; \
	unsigned int chunk_y  = y / CHUNK_SIZE; \
	unsigned int chunk_i = chunk_x + (w->width * chunk_y); \
	BOOL good = TRUE; \
	if (x >= w->width || y >= w->height) \
	{ \
		LOG_WARN("Attempted to access out of range chunk at (%d, %d) in world %d", x, y, w->id); \
		good = FALSE; \
	} \
	struct chunk *chunk = w->chunks + chunk_i;


enum tile_type world_get_tile(struct world *w, unsigned int x, unsigned int y)
{
	GET_CHUNK(w, x, y);
	if (!good)
		return TILE_BLANK;

	unsigned int tile_x = x % CHUNK_SIZE;
	unsigned int tile_y = y % CHUNK_SIZE;
	unsigned int tile_i = tile_x + (CHUNK_SIZE * tile_y);

	return (enum tile_type) chunk->tiles[tile_i];
}

void world_set_tile(struct world *w, unsigned int x, unsigned int y, enum tile_type type)
{
	GET_CHUNK(w, x, y);
	if (!good)
		return;

	unsigned int tile_x = x % CHUNK_SIZE;
	unsigned int tile_y = y % CHUNK_SIZE;
	unsigned int tile_i = tile_x + (CHUNK_SIZE * tile_y);
	chunk->tiles[tile_i] = type;
}
