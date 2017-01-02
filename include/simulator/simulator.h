#ifndef SIMULATOR_H
#define SIMULATOR_H

struct simulator_state;
typedef unsigned int simulator_id;

void simulator_init(struct simulator_state **sim);

void simulator_step(struct simulator_state *sim);

void simulator_destroy(struct simulator_state **sim);

simulator_id simulator_get_id(struct simulator_state *sim);

#endif
