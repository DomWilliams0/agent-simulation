#ifndef WORLD_H
#define WORLD_H

#include "world/world_forward.h"

struct world;

void world_create(struct world **w);

void world_destroy(struct world **w);

void world_step(struct world *w);

void world_create_entity(struct world *w, world_body *body);

#endif
