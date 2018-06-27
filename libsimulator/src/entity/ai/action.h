#ifndef SIM_ACTION_H
#define SIM_ACTION_H

#include <stdarg.h>
#include <entity/components.h>
#include "action_types.h"

// handy helper for generating methods for each derived class

// initialiser declaration
#define DERIVED_DECLARE_INIT(name_caps) \
 const struct ac_action AC_INIT_ ## name_caps;

// initialiser definition
#define DERIVED_DEFINE_INIT(name_caps, name_lower) \
	const struct ac_action AC_INIT_ ## name_caps = { \
		.type = AC_ ## name_caps, \
		.payload._vptr = { \
				.init = (void *) init_ ## name_lower, \
				.tick = (void *) tick_ ## name_lower, \
				.destroy = (void *) destroy_ ## name_lower, \
		} \
	};


enum ac_status
{
	/*
	AC_STATUS_NONE = 0, // not started yet
	AC_STATUS_STARTING, // start in progress
	AC_STATUS_RUNNING,  // in progress
	AC_STATUS_STOPPING, // stop in progress
	AC_STATUS_STOPPED,  // done
	*/
	AC_STATUS_RUNNING = 1,
	AC_STATUS_SUCCESS,
	AC_STATUS_FAILURE,
};

typedef int ac_priority;

struct ac_action
{
	enum ac_type type;

	union
	{
		struct ac_vtable _vptr;
		struct ac_flee flee;
		struct ac_move_to move_to;
		struct ac_idle idle;
		struct ac_follow follow;
	} payload;
};

struct ecs_comp_steer;
struct ecs;
struct ac_tick_arg
{
	ecs_id this;
	struct ecs_comp_steer *steer_out;
	struct ecs *ecs;
};

// higher is better
ac_priority ac_priority_of(struct ac_action *action);

// called before tick
void ac_init(struct ac_action *a, ...);

enum ac_status ac_tick(struct ac_action *a, struct ac_tick_arg *out);

void ac_destroy(struct ac_action *a);

// action defaults
DERIVED_DECLARE_INIT(FLEE)
DERIVED_DECLARE_INIT(MOVE_TO)
DERIVED_DECLARE_INIT(IDLE)
DERIVED_DECLARE_INIT(FOLLOW)

#endif