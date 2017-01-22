#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "graphics.h"
#include "world/world.h" // TODO temporary for HUMAN_RADIUS

#include "util/memory.h"
#include "util/log.h"
#include "util/util.h"

struct graphics_ctx
{
	ALLEGRO_DISPLAY *display;
};

struct
{
	ALLEGRO_COLOR BG;
} graphics_colours;

MODULE_IMPLEMENT(struct graphics_ctx, "graphics context",
		graphics_init,
		{
			if ((new_instance->display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)) == NULL)
			{
				LOG_INFO("Failed to create display");
				return NULL;
			}

			graphics_colours.BG = al_map_rgb(17, 17, 19);
		},
		graphics_destroy,
		{
			al_destroy_display(instance->display);
		})

ALLEGRO_EVENT_SOURCE *graphics_get_display_event_source(struct graphics_ctx *ctx)
{
	return al_get_display_event_source(ctx->display);
}

void graphics_start(struct graphics_ctx *ctx)
{
	UNUSED(ctx);

	al_clear_to_color(graphics_colours.BG);
}

void graphics_draw_human(struct graphics_ctx *ctx, double x, double y, ALLEGRO_COLOR *colour)
{
	UNUSED(ctx);

	// TODO scale view
	al_draw_circle(x, y, HUMAN_RADIUS, *colour, 1.f);
}

void graphics_end(struct graphics_ctx *ctx)
{
	UNUSED(ctx);

	al_flip_display();
}
