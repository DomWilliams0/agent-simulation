#ifndef SIM_ROAD_H
#define SIM_ROAD_H

#include <chipmunk/chipmunk.h>

struct road
{
	cpVect a, b;
	float thickness;
};

#endif