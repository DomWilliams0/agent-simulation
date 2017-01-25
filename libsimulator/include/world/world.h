#ifndef WORLD_H
#define WORLD_H

#include "world/world_forward.h"
#include "util/util.h"

#define HUMAN_MASS (10.0)
#define CHUNK_SIZE (32)

struct simulator;

struct position
{
	float x, y;
};

struct world_parameters
{
	int width;
	int height;
	char *file_path;
};

MODULE_DECLARE(struct world, world_create, world_destroy);

int world_get_width(struct world *w);
int world_get_height(struct world *w);
char *world_get_file_path(struct world *w);
unsigned int world_get_id(struct world *w);

void world_step(struct world *w);

world_body world_create_entity(struct world *w);
void world_get_position(world_body body, struct position *pos);
void world_set_position(world_body body, struct position *pos);

#endif
