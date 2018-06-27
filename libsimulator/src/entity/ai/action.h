#ifndef SIM_ACTION_H
#define SIM_ACTION_H

#include <stdarg.h>
#include "action_types.h"

enum ac_status
{
	AC_STATUS_NONE = 0, // not started yet
	AC_STATUS_STARTING, // start in progress
	AC_STATUS_RUNNING,  // in progress
	AC_STATUS_STOPPING, // stop in progress
	AC_STATUS_STOPPED,  // done
};

typedef int ac_priority;

struct ac_action
{
	enum ac_type type;

	union
	{
		struct ac_flee flee;
		struct ac_move_to move_to;
		struct ac_idle idle;
	};
};

// higher is better
ac_priority ac_priority_of(struct ac_action *action);
#endif