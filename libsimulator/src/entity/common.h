#ifndef SIM_ENTITY_COMMON_H
#define SIM_ENTITY_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include "util/util.h"

#define HUMAN_FRICTION     (0.f)
#define WORLD_DAMPING      (0.05f) // this limits the maximum speed
#define HUMAN_ACCELERATION (100.f)
#define VELOCITY_MINIMUM   (0.001f)
#define HUMAN_RADIUS       (0.3)


typedef uint8_t human_age;

enum gender
{
	GENDER_MALE = 1,
	GENDER_FEMALE,
};

#endif
