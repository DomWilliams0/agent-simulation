#ifndef ENTITY_H
#define ENTITY_H

#include "util/bool.h"

// TODO entity IDs will be unique across all worlds
typedef unsigned int entity_id;
typedef unsigned int human_age;

typedef enum { MALE = 1, FEMALE } human_gender;

struct entity_ctx;
struct simulator_state;

void entity_init_context(struct entity_ctx **ctx);

void entity_destroy_context(struct entity_ctx **ctx);

struct entity_ctx *entity_get_context(struct simulator_state *sim);

entity_id entity_create(struct entity_ctx *ctx);

void entity_destroy(struct entity_ctx *ctx, entity_id e);

BOOL entity_is_alive(struct entity_ctx *ctx, entity_id e);

entity_id entity_get_count(struct entity_ctx *ctx);

entity_id entity_get_max_count(struct entity_ctx *ctx);

entity_id entity_get_iterator(struct entity_ctx *ctx);

entity_id entity_get_next(struct entity_ctx *ctx, entity_id e);

#endif
