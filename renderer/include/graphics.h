#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "util/util.h"

#define WINDOW_WIDTH  (600)
#define WINDOW_HEIGHT (400)

// shhh
typedef struct ALLEGRO_EVENT_SOURCE ALLEGRO_EVENT_SOURCE;

struct colour
{
	float r, g, b;
};

struct camera_movement
{
	float move_hor;
	float move_ver;
	float zoom;
};

struct world;
struct component_human;

MODULE_DECLARE(struct graphics_ctx, graphics_init, graphics_destroy)

ALLEGRO_EVENT_SOURCE *graphics_get_display_event_source(struct graphics_ctx *ctx);

void graphics_start(struct graphics_ctx *ctx);

void graphics_draw_world(struct world *world);

void graphics_draw_human(float x, float y, struct component_human *human);

void graphics_end();

void graphics_resize(struct graphics_ctx *ctx, int w, int h);

void graphics_update_camera(struct graphics_ctx *ctx, struct camera_movement changes);

#endif
