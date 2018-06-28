#ifndef SIM_STEERING_H
#define SIM_STEERING_H

#include <chipmunk/cpVect.h>
#include "components_decl.h"

struct ecs;
ECS_COMP(steer);

enum st_type
{
	ST_NONE = 0,
	ST_SEEK,
	ST_FLEE,
	ST_ARRIVE,
};

void st_apply(ECS_COMP(steer) *steer, cpVect current_pos, cpVect *velocity_out);

// realise steering components -> physical movement
void st_system_tick(struct ecs *ecs);

#endif
