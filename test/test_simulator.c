#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "simulator/simulator.h"
#include "entity/entity.h"

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

static int teardown_remove_all_entities(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;

	entity_id i = entity_get_iterator(sim);

	while (entity_is_valid(i))
	{
		// cache next
		entity_id next = entity_get_next(i);

		// destroy current
		entity_destroy(i);

		i = next;
	}

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

static void test_entity_creation(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;

	// start with zero
	assert_int_equal(0, entity_get_count(sim));

	// add entity
	entity_id e = entity_create(sim);
	assert_true(entity_is_valid(e));

	// ensure it was added
	assert_int_equal(1, entity_get_count(sim));
}

static void test_entity_creation_max(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;

	entity_id max_count = entity_get_max_count(sim);

	// create max_count entities
	entity_id e;
	while (max_count--)
	{
		e = entity_create(sim);
		assert_true(entity_is_valid(e));
	}

	// next allocation should fail
	e = entity_create(sim);
	assert_false(entity_is_valid(e));

	// free one space
	entity_destroy(entity_get_iterator(sim));
	e = entity_create(sim);
	assert_true(entity_is_valid(e));

	// full again
	e = entity_create(sim);
	assert_false(entity_is_valid(e));
}

static void test_entity_iteration(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;

	// add entities
	entity_id e1, e2, e3;

	e1 = entity_create(sim);
	e2 = entity_create(sim);
	e3 = entity_create(sim);
	assert_true(entity_is_valid(e1));
	assert_true(entity_is_valid(e2));
	assert_true(entity_is_valid(e3));

	// iterate
	entity_id iterator = entity_get_iterator(sim);

	for (int i = 0; i < 3; ++i)
	{
		assert_true(entity_is_valid(iterator));
		iterator = entity_get_next(iterator);
	}

	// should be at end now
	assert_false(entity_is_valid(iterator));
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
		cmocka_unit_test(test_step),

		cmocka_unit_test_teardown(test_entity_creation, teardown_remove_all_entities),
		cmocka_unit_test_teardown(test_entity_creation_max, teardown_remove_all_entities),
		cmocka_unit_test_teardown(test_entity_iteration, teardown_remove_all_entities),
	};

	return cmocka_run_group_tests(tests, setup, teardown);
}

