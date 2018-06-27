#include <stdio.h>
#include "tests.h"
#include "entity/ai/action_stack.h"

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
}