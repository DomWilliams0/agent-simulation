#ifndef SIM_ACTION_TYPES_H
#define SIM_ACTION_TYPES_H

enum ac_type
{
	AC_FLEE,
	AC_MOVE_TO,
	AC_IDLE,
/*	AC_FOLLOW,
	AC_ATTACK,*/
};

// must be first field of every action
struct ac_vtable
{
	void (*init)(void *, va_list);
	enum ac_status (*tick)(void *);
	void (*destroy)(void *);
};

// TODO replace with real handle
typedef int entity_handle;

struct ac_flee
{
	struct ac_vtable _vptr;
	entity_handle target;
};

struct ac_move_to
{
	struct ac_vtable _vptr;
	double target[2];
};

struct ac_idle
{
	struct ac_vtable _vptr;
};

#endif