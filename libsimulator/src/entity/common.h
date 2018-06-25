#ifndef ENTITY_COMMON_H
#define ENTITY_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include "util/util.h"

#define HUMAN_DAMPING      (0.15f) // this limits the maximum speed
#define HUMAN_ACCELERATION (40.f)
#define VELOCITY_MINIMUM   (0.001f)
#define HUMAN_RADIUS       (0.3)


// TODO entity IDs will be unique across all worlds
typedef uint32_t entity_id;

typedef enum { MALE = 1, FEMALE } human_gender;

// TODO remove
typedef void entity_consumer(entity_id, void *);

#endif
