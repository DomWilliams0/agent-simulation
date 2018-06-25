#ifndef WORLD_MODULE_H
#define WORLD_MODULE_H

#include <chipmunk/chipmunk.h>
#include <stdint.h>

#include "world/world.h"

struct world
{
	cpSpace *space;

	coord width;
	coord height;
};

#endif
