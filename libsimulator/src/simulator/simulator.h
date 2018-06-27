#ifndef SIM_SIMULATOR_H
#define SIM_SIMULATOR_H

#include <stdint.h>

#include "util/util.h"
#include "entity/ecs.h"

MOD_DECLARE(simulator, {
	struct ecs ecs;
	struct world *world;
});

void simulator_populate(struct simulator *sim);

void simulator_step(struct simulator *sim);

#endif
