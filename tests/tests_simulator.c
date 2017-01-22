#include "helper.h"
#include "simulator/simulator.h"
#include "util/util.h"

UNIT_TEST(simulator_creation_destruction)
{
	UNUSED(state);
	struct simulator *sim = simulator_create(NULL);
	assert_non_null(sim);

	simulator_destroy(sim);
}

UNIT_TEST(simulator_step)
{
	struct simulator *sim = (struct simulator *)*state;
	simulator_step(sim);

	// ...
}

REGISTER_TEST(simulator_creation_destruction);
REGISTER_TEST(simulator_step);
