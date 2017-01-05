#include "simulator/simulator.h"
#include "util/log.h"

int main()
{
	LOG_INFO("Starting renderer");

	struct simulator_state *sim;
	simulator_init(&sim);
	// simulator_step(&sim);
	simulator_destroy(&sim);

	return 0;
}
