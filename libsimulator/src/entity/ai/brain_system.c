#include "brain_system.h"
#include "entity/ecs.h"
#include "action.h"

void br_system_tick(struct ecs *ecs)
{
	struct ecs_comp_brain *brains = ecs_all(ecs, ECS_COMP_BRAIN);
	struct ecs_comp_steer *steers = ecs_all(ecs, ECS_COMP_STEER);
	const ecs_mask mask = ECS_COMP_BRAIN | ECS_COMP_STEER;

	struct ac_action current;
	struct ac_tick_arg arg = {
			.ecs = ecs
	};
	for (ecs_id i = 0; i < ecs->count; ++i)
	{
		if (!ecs_has(ecs, i, mask))
			continue;

		struct ecs_comp_brain *b = &brains[i];
		struct ecs_comp_steer *s = &steers[i];

		// TODO process events
		ev_queue_process(&b->event_queue, {
			UNUSED(it);
		});
		ev_queue_clear_n_flip(&b->event_queue);

		// tick behaviour
		ac_stack_current(&b->action_stack, &current);
		arg.steer_out = s;
		arg.this = i;

		enum ac_status status = ac_tick(&current, &arg);
		if (status != AC_STATUS_RUNNING)
			ac_stack_pop(&b->action_stack);
		// TODO call end on old action, begin on new
	}

}
