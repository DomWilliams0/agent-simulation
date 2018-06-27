#include "action.h"
#include "util/log.h"

ac_priority ac_priority_of(struct ac_action *action)
{
	// TODO arbitrary and too coarse
	switch (action->type)
	{
		case AC_FLEE:
			return 5;
		case AC_MOVE_TO:
			return 1;
		case AC_IDLE:
			return 0;
	}
	LOG_ERROR("action not handled");
	return 0;
}
