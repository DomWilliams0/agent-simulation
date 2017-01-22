#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "util/util.h"

typedef unsigned int simulator_id;

MODULE_DECLARE(struct simulator, simulator_create, simulator_destroy)

void simulator_step(struct simulator *sim);

simulator_id simulator_get_id(struct simulator *sim);

struct world *simulator_get_world(struct simulator *sim);

void simulator_populate(struct simulator *sim);

#endif
