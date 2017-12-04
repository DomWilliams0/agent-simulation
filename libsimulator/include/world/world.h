#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#include "world/world_forward.h"
#include "util/util.h"
#include "util/bool.h"

#define HUMAN_MASS (10.0)
#define CHUNK_SIZE (32)
#define CHUNK_TILE_COUNT (CHUNK_SIZE * CHUNK_SIZE)

struct simulator;


typedef uint8_t tile;
typedef uint8_t world_id;
typedef uint32_t tile_coord;
typedef uint32_t chunk_coord;

struct world_parameters
{
	chunk_coord chunk_width;
	chunk_coord chunk_height;
};

enum tile_type
{
	TILE_BLANK = 0,
	TILE_GRASS,

	TILE_MAX
};

struct chunk
{
	tile tiles[CHUNK_TILE_COUNT];
};


MODULE_DECLARE(struct world, world_create, world_destroy);

struct world *world_get_world(struct simulator *sim); // TODO add support for multiple

chunk_coord world_get_chunk_width(struct world *w);
chunk_coord world_get_chunk_height(struct world *w);
world_id world_get_id(struct world *w);

void world_step(struct world *w);

// entities
world_body world_create_entity(struct world *w);
void world_get_position(world_body body, float pos[2]);
void world_set_position(world_body body, float pos[2]);

// tiles
enum tile_type world_get_tile(struct world *w, tile_coord x, tile_coord y);
void world_set_tile(struct world *w, tile_coord x, tile_coord y, enum tile_type type);

struct chunk *world_get_chunk_array(struct world *w, chunk_coord *chunk_count);
tile *world_get_chunk_tiles(struct chunk *chunk);

// serialization
BOOL world_save(struct world *w, const char * const file_path);
struct world *world_load(const char * const file_path);

#endif
