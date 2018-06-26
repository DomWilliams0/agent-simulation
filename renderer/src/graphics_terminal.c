#ifdef GRAPHICS_TERMINAL

#include <stddef.h>
#include "graphics.h"
#include "entity/components.h"
#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

struct graphics
{
	int placeholder;
};

static int last_entity;

MOD_INIT(graphics, { return 0; });
MOD_DESTROY(graphics, {});

void graphics_start(struct graphics *self)
{
	UNUSED(self);
}

void graphics_draw_world(struct world *world)
{
	UNUSED(world);
}

void graphics_draw_human(double x, double y, struct component_human *human)
{
	LOG_INFO("Entity %-3d : %s %3dy (%2.1f, %.01f)",
			last_entity, human->gender == MALE ? "  male" : "female", human->age, x, y);
	last_entity += 1;
}

void graphics_end(struct graphics *self)
{
	UNUSED(self);
		last_entity = 1;
	LOG_INFO("----");
}

void graphics_resize(struct graphics *self, int w, int h)
{
	UNUSED(self);
	UNUSED(w);
	UNUSED(h);
}

void graphics_update_camera(struct graphics *self, struct camera_movement changes)
{
	UNUSED(self);
	UNUSED(changes);
}

#endif
