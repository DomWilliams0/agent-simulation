#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "util/bool.h"

struct simulator_state;
typedef unsigned int simulator_id;

struct simulator_state *simulator_create();

void simulator_step(struct simulator_state *sim);

void simulator_destroy(struct simulator_state *sim);

simulator_id simulator_get_id(struct simulator_state *sim);

struct world *simulator_get_world(struct simulator_state *sim);

void simulator_populate(struct simulator_state *sim);

#endif
