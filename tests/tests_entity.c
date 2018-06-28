#include "entity/steering.h"
#include "entity/components.h"
#include "entity/ecs.h"
#include "tests.h"

void test_entity()
{
	struct ecs ecs;
	ecs_init(&ecs, NULL);

	ASSERT(ecs.count == 0);
	ecs_id e = ecs_new(&ecs);
	ASSERT(ecs.count == 1);
	ASSERT(ecs_is_alive(&ecs, e));

	ECS_COMP(physics) *p;
	ECS_COMP(human) *h, *h2;

	// components
	ASSERT(!ecs_has(&ecs, e, human));
	ecs_add(&ecs, e, human, h);
	ASSERT(ecs_has(&ecs, e, human));
	ASSERT(ecs_get(&ecs, e, human) == h);

	ASSERT(!ecs_has(&ecs, e, physics));
	ecs_add(&ecs, e, physics, p);
	ASSERT(ecs_has(&ecs, e, physics));

	ecs_remove(&ecs, e, physics);
	ASSERT(!ecs_has(&ecs, e, physics));
	ASSERT(!ecs_has(&ecs, e, physics));

	ecs_id other = ecs_new(&ecs);
	ecs_add(&ecs, other, human, h2);

	h->age = 20;
	h->gender = GENDER_MALE;
	h2->age = 80;
	h2->gender = GENDER_FEMALE;

	// ensure no memory overlap
	ASSERT(h->age == 20 && h->gender == GENDER_MALE);
	ASSERT(h2->age == 80 && h2->gender == GENDER_FEMALE);
}

