#include "world/world.h"
#include "collision.h"

void world_add_road(struct world *w, struct road *road)
{
	vec_push(&w->roads, *road);

	cpShape *line = cpSegmentShapeNew(cpSpaceGetStaticBody(w->space), road->a, road->b, road->thickness);
	cpShapeSetFriction(line, 0.2);
	cpSpaceAddShape(w->space, line);
	cpShapeSetCollisionType(line, CG_ROAD_BOUNDARY);
	cpShapeSetSensor(line, cpTrue);
}
