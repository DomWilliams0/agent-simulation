#include <stdlib.h>

#include "simulator/simulator.h"

struct simulator_state
{
	int placeholder;
};

void simulator_init(struct simulator_state **sim)
{
	struct simulator_state *new_sim;
	new_sim = malloc(sizeof(struct simulator_state));

	new_sim->placeholder = 1;

	*sim = new_sim;
}

void simulator_step(struct simulator_state *sim)
{
	sim->placeholder += 1;
}

void simulator_destroy(struct simulator_state **sim)
{
	if (*sim)
		free(*sim);

	*sim = NULL;
}

int main(int argc, char **argv)
{

}
