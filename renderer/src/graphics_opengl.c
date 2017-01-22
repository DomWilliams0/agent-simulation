#ifdef GRAPHICS_OPENGL

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "graphics.h"

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

struct graphics_ctx
{
};

MODULE_IMPLEMENT(struct graphics_ctx, "OpenGL graphics context",
		graphics_init,
		{
			LOG_INFO("Not implemented");
		},
		graphics_destroy,
		{
		})

ALLEGRO_EVENT_SOURCE *graphics_get_display_event_source(struct graphics_ctx *ctx)
{
	UNUSED(ctx);

	return NULL;
}

void graphics_start(struct graphics_ctx *ctx)
{
	UNUSED(ctx);
}

void graphics_draw_human(struct graphics_ctx *ctx, double x, double y, ALLEGRO_COLOR *colour)
{
	UNUSED(ctx);
	UNUSED(x);
	UNUSED(y);
	UNUSED(colour);
}

void graphics_end(struct graphics_ctx *ctx)
{
	UNUSED(ctx);
}

#endif
