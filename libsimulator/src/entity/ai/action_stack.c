#include <assert.h>

#include "util/util.h"
#include "action_stack.h"

void ac_stack_init(struct ac_stack *stack, struct ac_action *default_action)
{
	vec_init(&stack->_stack);
	vec_reserve(&stack->_stack, AC_INITIAL_STACK_LENGTH);

	// should always be at the bottom of the stack
	assert(default_action);
	vec_push(&stack->_stack, *default_action);
}

void ac_stack_destroy(struct ac_stack *stack)
{
	vec_deinit(&stack->_stack);
}

void ac_stack_current(struct ac_stack *stack, struct ac_action *out)
{
	assert(stack->_stack.length > 0);
	*out = vec_last(&stack->_stack);
}

void ac_stack_push(struct ac_stack *stack, struct ac_action *action)
{
	ac_priority priority = ac_priority_of(action);

	// find insert pos
	int i = stack->_stack.length - 1;
	while (ac_priority_of(&stack->_stack.data[i]) > priority && i > 0)
	     i--;

	i++; // after the chosen element
	vec_insert(&stack->_stack, i, *action);
}

void ac_stack_pop(struct ac_stack *stack)
{
	struct ac_action top = vec_pop(&stack->_stack);
	UNUSED(top);
}
