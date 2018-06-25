#include <stddef.h>

#include "renderer.h"
#include "simulator/simulator.h"

int main()
{
	struct simulator sim;
	struct renderer renderer;

	if (simulator_init(&sim, NULL) != 0)
		return 1;

	if (renderer_init(&renderer, &sim) != 0)
		return 2;

	simulator_populate(&sim);
	renderer_start_loop(&renderer);

	renderer_destroy(&renderer);
	simulator_destroy(&sim);

	return 0;
}

