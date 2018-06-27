#include <entity/ai/action.h>
#include "tests.h"
#include "util/util.h"
#include "entity/ai/action_stack.h"
#include "entity/ai/action.h"

void test_polymorphism();

void test_action()
{
	struct ac_action default_action = {0};
	default_action.type = AC_IDLE;

	struct ac_stack stack;
	ac_stack_init(&stack, &default_action);

	// default when empty
	struct ac_action top = {0};
	ac_stack_current(&stack, &top);
	ASSERT(top.type == AC_IDLE);

	// push action
	struct ac_action action = {0};
	action.type = AC_MOVE_TO;
	ac_stack_push(&stack, &action);

	ac_stack_current(&stack, &top);
	ASSERT(top.type == AC_MOVE_TO);

	// pop it
	ac_stack_pop(&stack);
	ac_stack_current(&stack, &top);
	ASSERT(top.type == AC_IDLE);

	// push with priority
	action.type = AC_FLEE;
	ac_stack_push(&stack, &action);
	action.type = AC_MOVE_TO;
	ac_stack_push(&stack, &action);

	ac_stack_current(&stack, &top);
	ASSERT(top.type == AC_FLEE);

	ac_stack_destroy(&stack);

	test_polymorphism();
}

void test_polymorphism()
{
	struct ac_action action = AC_INIT_FLEE;
	ac_init(&action, 409);
	ASSERT(action.payload.flee.target == 409);

	struct ecs_comp_steer dummy;
	struct ac_tick_arg arg = {.steer_out = &dummy};
	enum ac_status status = ac_tick(&action, &arg);
	UNUSED(status);
	ASSERT(action.payload.flee.target == 409); // still kicking
	ac_destroy(&action);

	double pos[2] = {90.2, 10.44};
	action = AC_INIT_MOVE_TO;
	ac_init(&action, &pos);
	ASSERT_POS(action.payload.move_to.target, pos);
	ac_destroy(&action);
}