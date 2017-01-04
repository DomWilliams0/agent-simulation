#include "helper.h"
#include "entity/entity.h"

static void test_entity_creation_destruction(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	// start with zero
	assert_int_equal(0, entity_get_count(ctx));

	// add entity
	entity_id e = entity_create(ctx);
	assert_true(entity_is_alive(ctx, e));

	// ensure it was added
	assert_int_equal(1, entity_get_count(ctx));

	// remove again
	entity_destroy(ctx, e);
	assert_int_equal(0, entity_get_count(ctx));

	// do nothing if attempt to destroy again
	entity_destroy(ctx, e);
	assert_int_equal(0, entity_get_count(ctx));
}

static void test_entity_creation_max(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	entity_id max_count = entity_get_max_count(ctx);

	// create max_count entities
	entity_id e;
	assert_int_equal(0, entity_get_count(ctx));
	while (max_count--)
	{
		e = entity_create(ctx);
		assert_true(entity_is_alive(ctx, e));
	}

	// next allocation should fail
	e = entity_create(ctx);
	assert_false(entity_is_alive(ctx, e));

	// free one space
	entity_destroy(ctx, entity_get_iterator(ctx));
	e = entity_create(ctx);
	assert_true(entity_is_alive(ctx, e));

	// full again
	e = entity_create(ctx);
	assert_false(entity_is_alive(ctx, e));
}

static void test_entity_iteration(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	// add entities
	entity_id e1, e2, e3;

	assert_int_equal(0, entity_get_count(ctx));
	e1 = entity_create(ctx);
	e2 = entity_create(ctx);
	e3 = entity_create(ctx);
	assert_true(entity_is_alive(ctx, e1));
	assert_true(entity_is_alive(ctx, e2));
	assert_true(entity_is_alive(ctx, e3));

	// iterate
	entity_id iterator = entity_get_iterator(ctx);

	for (int i = 0; i < 3; ++i)
	{
		assert_true(entity_is_alive(ctx, iterator));
		iterator = entity_get_next(ctx, iterator);
	}

	// should be at end now
	assert_false(entity_is_alive(ctx, iterator));
}

static void test_entity_is_alive(void **state)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	// start with zero
	assert_int_equal(0, entity_get_count(ctx));

	// add single entity
	entity_id e = entity_create(ctx);

	// ensure only this entity is valid
	assert_true(entity_is_alive(ctx, e));
	assert_false(entity_is_alive(ctx, e + 1));
	assert_false(entity_is_alive(ctx, e - 1));
	assert_false(entity_is_alive(ctx, -e));
}


REGISTER_TEST_TEARDOWN(test_entity_creation_destruction, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(test_entity_creation_max, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(test_entity_iteration, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(test_entity_is_alive, teardown_remove_all_entities);
