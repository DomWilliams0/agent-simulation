#include "helper.h"
#include "simulator/simulator.h"
#include "util/util.h"

UNIT_TEST(simulator_creation_destruction)
{
	UNUSED(state);
	struct simulator_state *sim = simulator_create(NULL);
	assert_non_null(sim);

	simulator_destroy(sim);
}

UNIT_TEST(simulator_step)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	simulator_step(sim);

	// ...
}

REGISTER_TEST(simulator_creation_destruction);
REGISTER_TEST(simulator_step);
