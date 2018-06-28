#ifndef SIM_COLLISION_H
#define SIM_COLLISION_H

#include <chipmunk/chipmunk_structs.h>

enum collision_group
{
	CG_HUMAN_SENSOR =  1 << 0,
	CG_ROAD_BOUNDARY = 1 << 1,

//	CG_ALL = ~0,
//	CG_ALL_PHYSICAL = ~CG_HUMAN_SENSOR,
};

static const cpShapeFilter COLLISION_HUMAN_SENSOR = { CP_NO_GROUP, CG_HUMAN_SENSOR, CG_ROAD_BOUNDARY };
static const cpShapeFilter COLLISION_ROAD_BOUNDARY = { CP_NO_GROUP, CG_ROAD_BOUNDARY, CG_HUMAN_SENSOR };

#endif