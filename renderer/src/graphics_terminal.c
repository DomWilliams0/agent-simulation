#ifdef GRAPHICS_TERMINAL

#include "graphics.h"
#include "entity/components.h"
#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

struct graphics_ctx
{
	int placeholder;
};

static int last_entity;

MODULE_IMPLEMENT(struct graphics_ctx, "terminal context",
		graphics_init,
		{
		},
		graphics_destroy,
		{
		})


void graphics_start(struct graphics_ctx *ctx)
{
	UNUSED(ctx);
}

void graphics_draw_world(struct world *world)
{
	UNUSED(world);
}

void graphics_draw_human(float x, float y, struct component_human *human)
{
	LOG_INFO("Entity %-3d : %s %3dy (%2.1f, %.01f)",
			last_entity, human->gender == MALE ? "  male" : "female", human->age, x, y);
	last_entity += 1;
}

void graphics_end(struct graphics_ctx *ctx)
{
	UNUSED(ctx);
		last_entity = 1;
	LOG_INFO("----");
}

void graphics_resize(struct graphics_ctx *ctx, int w, int h)
{
	UNUSED(ctx);
	UNUSED(w);
	UNUSED(h);
}

void graphics_update_camera(struct graphics_ctx *ctx, struct camera_movement changes)
{
	UNUSED(ctx);
	UNUSED(changes);
}

#endif
