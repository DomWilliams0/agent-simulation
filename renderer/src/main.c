#include <stddef.h>

#include "renderer.h"
#include "simulator/simulator.h"

int main()
{
	struct simulator_state *sim;
	struct renderer_state *renderer;

	if ((sim = simulator_create()) == NULL)
		return 1;

	if ((renderer = renderer_create(sim)) == NULL)
		return 2;

	renderer_start_loop(renderer);

	renderer_destroy(renderer);
	simulator_destroy(sim);

	return 0;
}

