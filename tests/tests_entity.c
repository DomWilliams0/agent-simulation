#include "entity/steering.h"
#include "entity/components.h"
#include "entity/ecs.h"
#include "util/util.h"
#include "tests.h"

void test_entity()
{
	struct ecs ecs;
	ecs_init(&ecs, NULL);

	ASSERT(ecs.count == 0);
	ecs_id e = ecs_new(&ecs);
	ASSERT(ecs.count == 1);
	ASSERT(ecs_is_alive(&ecs, e));

	// components
	ASSERT(!ecs_has(&ecs, e, ECS_COMP_HUMAN));
	ecs_add(&ecs, e, ECS_COMP_HUMAN);
	ASSERT(ecs_has(&ecs, e, ECS_COMP_HUMAN));
	struct ecs_comp_human *human = ecs_get(&ecs, e, ECS_COMP_HUMAN, struct ecs_comp_human);
	ASSERT(ecs_get(&ecs, e, ECS_COMP_HUMAN, struct ecs_comp_human) == human);

	ASSERT(!ecs_has(&ecs, e, ECS_COMP_PHYSICS));
	ecs_add(&ecs, e, ECS_COMP_PHYSICS);
	ecs_add(&ecs, e, ECS_COMP_PHYSICS); // multiple adds are fine
	ASSERT(ecs_has(&ecs, e, ECS_COMP_PHYSICS));

	ecs_remove(&ecs, e, ECS_COMP_PHYSICS);
	ASSERT(!ecs_has(&ecs, e, ECS_COMP_PHYSICS));
	ecs_remove(&ecs, e, ECS_COMP_PHYSICS); // multiple removes too
	ASSERT(!ecs_has(&ecs, e, ECS_COMP_PHYSICS));

	ecs_id other = ecs_new(&ecs);
	ecs_add(&ecs, other, ECS_COMP_HUMAN);
	struct ecs_comp_human *other_human = ecs_get(&ecs, other, ECS_COMP_HUMAN, struct ecs_comp_human);

	human->age = 20;
	human->gender = GENDER_MALE;
	other_human->age = 80;
	other_human->gender = GENDER_FEMALE;

	// ensure no memory overlap
	ASSERT(human->age == 20 && human->gender == GENDER_MALE);
	ASSERT(other_human->age == 80 && other_human->gender == GENDER_FEMALE);
}

