#include "helper.h"
#include "entity/components.h"
#include "entity/entity.h"
#include "util/util.h"

UNIT_TEST(entity_creation_destruction)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	// start with zero
	assert_int_equal(0, entity_get_count(ctx));

	// add entity
	entity_id e = entity_create(ctx);

	// ensure it was added
	assert_int_equal(1, entity_get_count(ctx));
	assert_true(entity_is_alive(ctx, e));

	// remove again
	entity_destroy(ctx, e);
	assert_int_equal(0, entity_get_count(ctx));
}

UNIT_TEST(entity_creation_max)
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
	entity_id last_valid_entity = e;

	// next allocation should fail
	e = entity_create(ctx);
	assert_false(entity_is_alive(ctx, e));

	// free one space
	entity_destroy(ctx, last_valid_entity);
	e = entity_create(ctx);
	assert_true(entity_is_alive(ctx, e));

	// full again
	e = entity_create(ctx);
	assert_false(entity_is_alive(ctx, e));
}

static void iterator_func(entity_id e, void *arg)
{
	assert_true(entity_is_alive((struct entity_ctx *)arg, e));
}

static void iterator_func_fail(entity_id e, void *arg)
{
	UNUSED(arg);
	UNUSED(e);

	fail();
}

UNIT_TEST(entity_iteration)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);

	// no entities
	assert_false(entity_is_alive(ctx, entity_get_first(ctx)));
	entity_foreach(ctx, iterator_func_fail, NULL);

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
	entity_id e = entity_get_first(ctx);
	for (int i = 0; i < 3; ++i, ++e)
	{
		assert_true(entity_is_alive(ctx, e));
	}

	// should be at end now
	assert_false(entity_is_alive(ctx, e));

	// do same with foreach
	entity_foreach(ctx, iterator_func, ctx);
}

UNIT_TEST(entity_is_alive)
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
}

UNIT_TEST(entity_component_add_remove)
{
	struct simulator_state *sim = (struct simulator_state *)*state;
	struct entity_ctx *ctx = entity_get_context(sim);
	entity_id e = entity_create(ctx);

	// no components to start
	assert_int_equal(COMPONENT_NONE, entity_get_component_mask(ctx, e));

	// add physics
	entity_add_component(ctx, e, COMPONENT_PHYSICS);
	assert_int_equal(COMPONENT_PHYSICS, entity_get_component_mask(ctx, e));
	assert_non_null(entity_get_component(ctx, e, COMPONENT_PHYSICS));

	// add human
	entity_add_component(ctx, e, COMPONENT_HUMAN);
	assert_int_equal(COMPONENT_PHYSICS | COMPONENT_HUMAN, entity_get_component_mask(ctx, e));
	assert_non_null(entity_get_component(ctx, e, COMPONENT_HUMAN));

	// adding multiple times doesn't matter
	entity_add_component(ctx, e, COMPONENT_HUMAN);
	assert_int_equal(COMPONENT_PHYSICS | COMPONENT_HUMAN, entity_get_component_mask(ctx, e));

	// remove physics
	entity_remove_component(ctx, e, COMPONENT_PHYSICS);
	assert_int_equal(COMPONENT_HUMAN, entity_get_component_mask(ctx, e));
	assert_null(entity_get_component(ctx, e, COMPONENT_PHYSICS));

	// removing multiple times doesn't matter
	entity_remove_component(ctx, e, COMPONENT_PHYSICS);
	assert_int_equal(COMPONENT_HUMAN, entity_get_component_mask(ctx, e));
}


REGISTER_TEST_TEARDOWN(entity_creation_destruction, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(entity_creation_max, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(entity_iteration, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(entity_is_alive, teardown_remove_all_entities);
REGISTER_TEST_TEARDOWN(entity_component_add_remove, teardown_remove_all_entities);
