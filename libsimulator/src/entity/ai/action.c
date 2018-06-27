#include <assert.h>
#include <entity/ecs.h>
#include <world/world.h>
#include "action.h"
#include "entity/components.h"
#include "util/log.h"

ac_priority ac_priority_of(struct ac_action *action)
{
	// TODO arbitrary and too coarse
	switch (action->type)
	{
		case AC_FLEE:
			return 5;
		case AC_MOVE_TO:
		case AC_FOLLOW:
			return 1;
		case AC_IDLE:
			return 0;
	}
	LOG_ERROR("Action %d not handled", action->type);
	return 0;
}

// base class vtable calls
void ac_init(struct ac_action *a, ...)
{
	va_list ap;
	va_start(ap, a);

	struct ac_vtable *vptr = (struct ac_vtable *) &a->payload;
	assert(vptr->init);
	vptr->init(a, ap);

	va_end(ap);
}

enum ac_status ac_tick(struct ac_action *a, struct ac_tick_arg *out)
{
	struct ac_vtable *vptr = (struct ac_vtable *) &a->payload;
	assert(vptr->tick);
	return vptr->tick(a, out);
}

void ac_destroy(struct ac_action *a)
{
	struct ac_vtable *vptr = (struct ac_vtable *) &a->payload;
	assert(vptr->destroy);
	return vptr->destroy(a);
}

// individual action implementations
// flee
static void init_flee(struct ac_action *action, va_list ap)
{
	struct ac_flee *this = &action->payload.flee;
	ecs_id target = va_arg(ap, ecs_id);
	double distance = va_arg(ap, double);

	this->target = target;
	this->desired_distance = distance;
}

static void destroy_flee(struct ac_action *action)
{
}

static enum ac_status tick_flee(struct ac_action *action, struct ac_tick_arg *out)
{
	struct ac_flee *this = &action->payload.flee;

	cpVect target = world_get_position((ecs_get(out->ecs,
	                                            this->target,
	                                            ECS_COMP_PHYSICS,
	                                            struct ecs_comp_physics))->body);
	cpVect my_pos = world_get_position((ecs_get(out->ecs,
	                                            out->this,
	                                            ECS_COMP_PHYSICS,
	                                            struct ecs_comp_physics))->body);

	if (cpvdistsq(target, my_pos) > this->desired_distance * this->desired_distance)
		return AC_STATUS_SUCCESS; // got away

	out->steer_out->type = ST_FLEE;
	out->steer_out->target = target;
	return AC_STATUS_RUNNING;
}

// move to
static void init_move_to(struct ac_action *action, va_list ap)
{
	struct ac_move_to *this = &action->payload.move_to;
	cpVect target = va_arg(ap, cpVect);
	this->target = target;
}

static void destroy_move_to(struct ac_action *action)
{
}

static enum ac_status tick_move_to(struct ac_action *action, struct ac_tick_arg *out)
{
	struct ac_move_to *this = &action->payload.move_to;

	// already set and arrived
	if (out->steer_out->type == ST_ARRIVE &&
			out->steer_out->arrived &&
			cpvdistsq(out->steer_out->target, this->target) < 0.0001)
	{
		return AC_STATUS_SUCCESS; // wahey
	}

	out->steer_out->type = ST_ARRIVE;
	out->steer_out->target = this->target;

	return AC_STATUS_RUNNING;
}

// idle
static void init_idle(struct ac_action *action, va_list ap)
{
}

static void destroy_idle(struct ac_action *action)
{
}

static enum ac_status tick_idle(struct ac_action *action, struct ac_tick_arg *out)
{
	out->steer_out->type = ST_NONE;
	return AC_STATUS_RUNNING;
}

// follow
static void init_follow(struct ac_action *action, va_list ap)
{
	struct ac_follow *this = &action->payload.follow;
	ecs_id target = va_arg(ap, ecs_id);

	this->target = target;
}

static void destroy_follow(struct ac_action *action)
{
}

static enum ac_status tick_follow(struct ac_action *action, struct ac_tick_arg *out)
{
	struct ac_follow *this = &action->payload.follow;

	cpVect target = world_get_position((ecs_get(out->ecs,
	                                            this->target,
	                                            ECS_COMP_PHYSICS,
	                                            struct ecs_comp_physics))->body);

	out->steer_out->type = ST_SEEK;
	out->steer_out->target = target;
	return AC_STATUS_RUNNING;
}

// init definitions
DERIVED_DEFINE_INIT(FLEE, flee)
DERIVED_DEFINE_INIT(MOVE_TO, move_to)
DERIVED_DEFINE_INIT(IDLE, idle)
DERIVED_DEFINE_INIT(FOLLOW, follow)
