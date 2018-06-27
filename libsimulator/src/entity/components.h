#ifndef SIM_COMPONENTS_H
#define SIM_COMPONENTS_H

#include "entity/common.h"
#include "entity/steering.h"
#include "world/world_forward.h"

enum ecs_component {
	ECS_COMP_PHYSICS = (1 << 0),
	ECS_COMP_HUMAN   = (1 << 1),
	ECS_COMP_STEER   = (1 << 2),
};

// physics body
struct ecs_comp_physics
{
	world_body body;
};

// human attributes
struct ecs_comp_human
{
	human_age age;
	enum gender gender;
};

// steering movement
struct ecs_comp_steer
{
	enum st_type type;
	double target[2]; // TODO use vec instead
	bool separation; // TODO bitflags for multiple behaviours
};

#endif
