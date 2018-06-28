#include <world/collision.h>
#include "sensor_system.h"
#include "entity/ecs.h"

float sense_danger(ECS_COMP(physics) *physics, enum cm_direction direction, float len)
{
	cpVect start = world_get_position(physics->body);
	cpVect sensor_direction = cpvmult(cpvforangle(cm_direction_angle(direction)), len); // TODO precompute this
	cpVect end = cpvadd(start, sensor_direction);

	cpSegmentQueryInfo query;
	// TODO correct filter
	cpShape *target = cpSpaceSegmentQueryFirst(cpBodyGetSpace(physics->body),
	                         start, end,
	                         0, COLLISION_HUMAN_SENSOR, &query);
	if (target == NULL)
		return 0;

	// roads are dangerous
	if (cpShapeGetCollisionType(target) == CG_ROAD_BOUNDARY)
		return 0.2f;

	// TODO default?
	return 0.f;
}

void sense_dangers(ECS_COMP(sensors) *sensors, ECS_COMP(physics) *physics, ECS_COMP(steer) *steer)
{
	for (enum cm_direction dir = CM_DIRECTION_FIRST; dir < CM_DIRECTION_LAST; ++dir)
	{
		float result = sense_danger(physics, dir, sensors->length);
		if (result > 0)
			cm_add(&steer->ctx_map, CM_DANGER, dir, result);
	}
}

void se_system_tick(struct ecs *ecs)
{
	ECS_COMP(sensors) *sensors = ecs_all(ecs, sensors);
	ECS_COMP(physics) *physics = ecs_all(ecs, physics);
	ECS_COMP(steer) *steers = ecs_all(ecs, steer);

	const ecs_mask mask = ECS_COMP_MASK(sensors) | ECS_COMP_MASK(physics) | ECS_COMP_MASK(steer);

	for (ecs_id i = 0; i < ecs->count; ++i)
	{
		if (ecs_has_mask(ecs, i, mask))
			sense_dangers(sensors + i, physics + i, steers + i);
	}
}