#include "helper.h"
#include "simulator/simulator.h"

UNIT_TEST(creation_destruction)
{
	struct simulator_state *sim;
	simulator_init(&sim);
	assert_non_null(sim);

	simulator_destroy(&sim);
	assert_null(sim);
}

UNIT_TEST(step)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	simulator_step(sim);

	// ...
}

REGISTER_TEST(creation_destruction);
REGISTER_TEST(step);
