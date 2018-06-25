#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>

#include "util/util.h"
#include "entity/entity.h"

MOD_DECLARE(simulator, {
	struct entities entities;
	struct world *world;
});

void simulator_populate(struct simulator *sim);

void simulator_step(struct simulator *sim);

#endif
