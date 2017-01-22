#include <stddef.h>

#include "renderer.h"
#include "simulator/simulator.h"

int main()
{
	struct simulator *sim;
	struct renderer *renderer;

	if ((sim = simulator_create(NULL)) == NULL)
		return 1;

	if ((renderer = renderer_create(sim)) == NULL)
		return 2;

	simulator_populate(sim);
	renderer_start_loop(renderer);

	renderer_destroy(renderer);
	simulator_destroy(sim);

	return 0;
}

