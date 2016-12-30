#ifndef SIMULATOR_H
#define SIMULATOR_H

struct simulator_state;

void simulator_init(struct simulator_state **sim);

void simulator_step(struct simulator_state *sim);

void simulator_destroy(struct simulator_state **sim);

#endif
