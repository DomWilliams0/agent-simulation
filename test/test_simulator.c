#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "simulator/simulator.h"

static int setup(void **state)
{
	struct simulator_state *sim = NULL;
	simulator_init(&sim);
	if (sim == NULL)
		return 1;

	*state = sim;
	return 0;
}

static int teardown(void **state)
{
	simulator_destroy((struct simulator_state **) state);
	return 0;
}

static void test_creation_deletion(void **state)
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

int main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(test_creation_deletion),
		cmocka_unit_test(test_step)
	};

	return cmocka_run_group_tests(tests, setup, teardown);
}

