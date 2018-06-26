#include "entity/steering.h"
#include "entity/components.h"
#include "entity/entity.h"
#include "util/util.h"
#include "tests.h"

void test_entity()
{
	struct entities entities;
	entities_init(&entities, NULL);
	struct entities *ctx = &entities;

	// life cycle
	ASSERT(entities.count == 0);
	entity_id e = entity_create(ctx);

	ASSERT(entities.count == 1);
	ASSERT(entity_is_alive(ctx, e));

	entity_destroy(ctx, e);
	ASSERT(!entity_is_alive(ctx, e));
	ASSERT(entities.count == 0);

	// components
	e = entity_create(ctx);
	ASSERT(COMPONENT_NONE == entity_get_component_mask(ctx, e));

	entity_add_component(ctx, e, COMPONENT_PHYSICS);
	ASSERT(COMPONENT_PHYSICS == entity_get_component_mask(ctx, e));
	ASSERT(entity_get_component(ctx, e, COMPONENT_PHYSICS) != NULL);
	ASSERT(entity_get_component(ctx, e, COMPONENT_HUMAN) == NULL);

	entity_add_component(ctx, e, COMPONENT_HUMAN);
	ASSERT((COMPONENT_PHYSICS | COMPONENT_HUMAN) == entity_get_component_mask(ctx, e));
	ASSERT(entity_get_component(ctx, e, COMPONENT_HUMAN) != NULL);
	ASSERT(entity_get_component(ctx, e, COMPONENT_PHYSICS) != NULL);

	entity_add_component(ctx, e, COMPONENT_HUMAN); // adding multiple times doesn't hurt
	ASSERT((COMPONENT_PHYSICS | COMPONENT_HUMAN) == entity_get_component_mask(ctx, e));
	ASSERT(entity_get_component(ctx, e, COMPONENT_HUMAN) != NULL);

	entity_remove_component(ctx, e, COMPONENT_PHYSICS);
	ASSERT(COMPONENT_HUMAN == entity_get_component_mask(ctx, e));
	ASSERT(entity_get_component(ctx, e, COMPONENT_HUMAN) != NULL);
	ASSERT(entity_get_component(ctx, e, COMPONENT_PHYSICS) == NULL);

	entity_remove_component(ctx, e, COMPONENT_PHYSICS); // multiple removes don't hurt either
	ASSERT(entity_get_component(ctx, e, COMPONENT_HUMAN) != NULL);
	ASSERT(entity_get_component(ctx, e, COMPONENT_PHYSICS) == NULL);

	// TODO test destroying with a selection of components

	entity_id e1 = entity_create(ctx);
	entity_id e2 = entity_create(ctx);

	struct component_human *human1 = entity_add_component(ctx, e1, COMPONENT_HUMAN);
	struct component_human *human1_get = entity_get_component(ctx, e1, COMPONENT_HUMAN);
	struct component_human *human1_arr =
			(struct component_human *) entity_get_component_array(ctx, COMPONENT_HUMAN) + e1;

	struct component_human *human2 = entity_add_component(ctx, e2, COMPONENT_HUMAN);
	struct component_human *human2_get = entity_get_component(ctx, e2, COMPONENT_HUMAN);
	struct component_human *human2_arr =
			(struct component_human *) entity_get_component_array(ctx, COMPONENT_HUMAN) + e2;

	ASSERT(human1 == human1_get && human1_get == human1_arr);
	ASSERT(human2 == human2_get && human2_get == human2_arr);

	// ensure no memory overlap
	human1->age = 20;
	human2->age = 80;
	ASSERT(human1->age == 20);
	ASSERT(human2->age == 80);

	// steering
	struct component_steer *steer = entity_add_component(ctx, e, COMPONENT_STEER);
	steer->type = STEERING_PATH_FOLLOW;
	steer->separation = false;

	steering_path_set(steer, NULL, 0); // shouldn't crash with null path

	// path setting
	double path[3][2] = {{4, 5},
	                     {6, 7},
	                     {8, 9}};
	steering_path_set(steer, (double *) path, 3);

	// ensure path was set correctly
	struct steering_path_waypoint *waypoint = steer->path_front;

	for (int i = 0; i < 3; ++i)
	{
		ASSERT(waypoint != NULL);
		ASSERT_POS(waypoint->pos, path[i]);
		waypoint = waypoint->next;
	}
	ASSERT(waypoint == NULL);

	// add one extra
	double extra[2] = {20, 40};
	steering_path_add(steer, extra);

	// ensure it was added to the end
	struct steering_path_waypoint *end = steer->path_end;
	ASSERT(end != NULL);
	ASSERT_POS(end->pos, extra);

	// pop from the front
	bool popped = steering_path_pop(steer);
	ASSERT(popped);
	ASSERT(steer->path_front != NULL);
	ASSERT_POS(steer->path_front->pos, path[1]);

	// exhaust
	for (int i = 0; i < 2; ++i)
		ASSERT(steering_path_pop(steer));

	ASSERT(!steering_path_pop(steer));
	ASSERT(steer->path_front == NULL);
	ASSERT(steer->path_end == NULL);
}

