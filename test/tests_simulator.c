#include "helper.h"
#include "simulator/simulator.h"

static void test_creation_destruction(void **state)
{
	struct simulator_state *sim;
	simulator_init(&sim);
	assert_non_null(sim);

	simulator_destroy(&sim);
	assert_null(sim);
}

static void test_step(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	simulator_step(sim);

	// ...
}

REGISTER_TEST(test_creation_destruction);
REGISTER_TEST(test_step);
