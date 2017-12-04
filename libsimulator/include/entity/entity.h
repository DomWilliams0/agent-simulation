#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#include "util/bool.h"
#include "util/util.h"

#define HUMAN_DAMPING      (0.15f) // this limits the maximum speed
#define HUMAN_ACCELERATION (40.f)
#define VELOCITY_MINIMUM   (0.001f)

// TODO entity IDs will be unique across all worlds
typedef uint32_t entity_id;
typedef uint8_t human_age;

typedef enum { MALE = 1, FEMALE } human_gender;

struct simulator;

typedef void entity_consumer(entity_id, void *);

MODULE_DECLARE(struct entity_ctx, entity_create_context, entity_destroy_context)

struct entity_ctx *entity_get_context(struct simulator *sim);

entity_id entity_create(struct entity_ctx *ctx);

void entity_destroy(struct entity_ctx *ctx, entity_id e);

BOOL entity_is_alive(struct entity_ctx *ctx, entity_id e);

entity_id entity_get_first(struct entity_ctx *ctx);

entity_id entity_get_count(struct entity_ctx *ctx);

entity_id entity_get_max_count(struct entity_ctx *ctx);

void entity_foreach(struct entity_ctx *ctx, entity_consumer *func, void *arg);

#endif
