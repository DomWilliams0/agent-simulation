#ifndef SIM_ACTION_STACK_H
#define SIM_ACTION_STACK_H

#define AC_INITIAL_STACK_LENGTH (8)

#include "util/vec.h"

typedef vec_t(struct ac_action) ac_stack_t;
struct action;

struct ac_stack
{
	ac_stack_t _stack;
};

// default action is copied
void ac_stack_init(struct ac_stack *stack, struct ac_action *default_action);

void ac_stack_destroy(struct ac_stack *stack);

// top of stack, or default action if none
void ac_stack_current(struct ac_stack *stack, struct ac_action *out);

// pushes onto stack wrt priority of given action
// action is copied
// TODO should it always be sorted? or only for interrupting actions
// TODO add is_interrupting flag to actions?
void ac_stack_push(struct ac_stack *stack, struct ac_action *action);

// top action must have indicated that it has finished
void ac_stack_pop(struct ac_stack *stack);
// TODO abort current/given action

#endif