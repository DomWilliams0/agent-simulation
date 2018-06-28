#include <entity/ai/action.h>
#include "components.h"

// already zeroed beforehand

ECS_COMP_INIT_DEFINE(physics, {}, {})

ECS_COMP_INIT_DEFINE(human, {
	comp->gender = GENDER_MALE;
}, {})

ECS_COMP_INIT_DEFINE(steer, {
	comp->type = ST_NONE;
}, {})

ECS_COMP_INIT_DEFINE(brain, {
	struct ac_action default_command = AC_INIT_IDLE;
	ac_stack_init(&comp->action_stack, &default_command);
	ev_queue_init(&comp->event_queue);
}, {
	ac_stack_destroy(&comp->action_stack);
	ev_queue_destroy(&comp->event_queue);
})
