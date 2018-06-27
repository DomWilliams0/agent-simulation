#ifndef SIM_ACTION_TYPES_H
#define SIM_ACTION_TYPES_H

#include "entity/common.h"

enum ac_type
{
	AC_FLEE,
	AC_MOVE_TO,
	AC_IDLE,
	AC_FOLLOW,
//	AC_ATTACK,
};

// must be first field of every action
struct ac_vtable
{
	void (*init)(void *, va_list);
	enum ac_status (*tick)(void *, struct ac_tick_arg *);
	void (*destroy)(void *);
};

struct ac_flee
{
	struct ac_vtable _vptr;
	ecs_id target;
	double desired_distance;
};

struct ac_follow
{
	struct ac_vtable _vptr;
	ecs_id target;
};

struct ac_move_to
{
	struct ac_vtable _vptr;
	cpVect target;
};

struct ac_idle
{
	struct ac_vtable _vptr;
};

#endif