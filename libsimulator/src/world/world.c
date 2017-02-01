#include <math.h>

#include "util/memory.h"
#include "util/bool.h"
#include "util/log.h"
#include "util/util.h"
#include "util/constants.h"

#include "world/world.h"
#include "world/internal/physics.h"
#include "world/internal/world.h"
#include "entity/steering.h"
#include "entity/entity.h"

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

unsigned int world_get_chunk_width(struct world *w)
{
	return w->chunk_width;
}

unsigned int world_get_chunk_height(struct world *w)
{
	return w->chunk_height;
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
	dBodySetLinearDamping(b, HUMAN_DAMPING);

	// collision
	dGeomID geom = dCreateSphere(w->collision_space, HUMAN_RADIUS);
	dGeomSetBody(geom, b);

	return b;
}

void world_get_position(world_body body, position pos)
{
	const double *dpos = dBodyGetPosition(body);
	pos[0] = dpos[0];
	pos[1] = dpos[1];
}

void world_set_position(world_body body, position pos)
{
	dBodySetPosition(body, pos[0] + 0.5, pos[1] + 0.5, 0);
}

static BOOL load_params(struct world *world, struct world_parameters *params)
{
	if (params == NULL)
	{
		LOG_WARN("No parameters provided");
		return FALSE;
	}

	if (params->chunk_width == 0 || params->chunk_height == 0)
	{
		LOG_WARN("Width and height must be positive");
		return FALSE;
	}

	// currently unsupported
	if (params->file_path != NULL)
	{
		LOG_WARN("World loading currently not supported");
		return FALSE;
	}

	world->chunk_width = params->chunk_width;
	world->chunk_height = params->chunk_height;
	world->file_path = params->file_path;

	return TRUE;
}

static void load_terrain(struct world *world)
{
	safe_malloc(world->chunk_width * world->chunk_height * sizeof(struct chunk), &world->chunks);
}

#define GET_CHUNK(w, x, y) \
	unsigned int chunk_x  = x / CHUNK_SIZE; \
	unsigned int chunk_y  = y / CHUNK_SIZE; \
	unsigned int chunk_i = chunk_x + (w->chunk_width * chunk_y); \
	BOOL good = TRUE; \
	if (chunk_x >= w->chunk_width || chunk_y >= w->chunk_height) \
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

struct chunk *world_get_chunk_array(struct world *w, unsigned int *chunk_count)
{
	*chunk_count = w->chunk_width * w->chunk_height;
	return w->chunks;
}

tile *world_get_chunk_tiles(struct chunk *chunk)
{
	return chunk->tiles;
}
