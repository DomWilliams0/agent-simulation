#include <assert.h>
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

enum ac_status ac_tick(struct ac_action *a)
{
	struct ac_vtable *vptr = (struct ac_vtable *) &a->payload;
	assert(vptr->tick);
	return vptr->tick(a);
}

void ac_destroy(struct ac_action *a)
{
	struct ac_vtable *vptr = (struct ac_vtable *) &a->payload;
	assert(vptr->destroy);
	return vptr->destroy(a);
}

// individual action implementations
// flee
DERIVED_IMPL(flee, init, void, {
	entity_handle target = va_arg(ap, entity_handle);
	this->target = target;
})
DERIVED_IMPL(flee, destroy, void, {})

DERIVED_IMPL(flee, tick, enum ac_status, {
	return AC_STATUS_RUNNING;
})

// move to
DERIVED_IMPL(move_to, init, void, {
	double *target = va_arg(ap, double*);
	this->target[0] = target[0];
	this->target[1] = target[1];
});
DERIVED_IMPL(move_to, destroy, void, {})

DERIVED_IMPL(move_to, tick, enum ac_status, {
	return AC_STATUS_RUNNING;
})

// idle
DERIVED_IMPL(idle, init, void, {});
DERIVED_IMPL(idle, destroy, void, {})

DERIVED_IMPL(idle, tick, enum ac_status, {
	return AC_STATUS_RUNNING;
})

// init definitions
DERIVED_DEFINE_INIT(FLEE, flee)
DERIVED_DEFINE_INIT(MOVE_TO, move_to)
DERIVED_DEFINE_INIT(IDLE, idle)
