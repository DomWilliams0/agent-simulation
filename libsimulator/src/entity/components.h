#ifndef SIM_COMPONENTS_H
#define SIM_COMPONENTS_H

#include <event/queue.h>
#include <chipmunk/cpVect.h>
#include "entity/common.h"
#include "entity/steering.h"
#include "world/world_forward.h"
#include "ai/action_stack.h"
#include "event/event.h"
#include "components_decl.h"

// physics body
ECS_COMP_DEFINE(0, physics,
	world_body body;
)

// human attributes
ECS_COMP_DEFINE(1, human,
	human_age age;
	enum gender gender;
)

// steering movement
ECS_COMP_DEFINE(2, steer,
	enum st_type type;
	cpVect target;
	bool separation; // TODO bitflags for multiple behaviours

	union
	{
		bool arrived; // arrival only
	};
)

// behaviour coordination
ECS_COMP_DEFINE(3, brain,
	struct ac_stack action_stack;
	struct ev_queue event_queue;
);
#endif
