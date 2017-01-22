#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "util/util.h"

#define WINDOW_WIDTH  (400)
#define WINDOW_HEIGHT (600)

// shhh
typedef struct ALLEGRO_EVENT_SOURCE ALLEGRO_EVENT_SOURCE;
typedef struct ALLEGRO_COLOR ALLEGRO_COLOR;

MODULE_DECLARE(struct graphics_ctx, graphics_init, graphics_destroy)

ALLEGRO_EVENT_SOURCE *graphics_get_display_event_source(struct graphics_ctx *ctx);

void graphics_start(struct graphics_ctx *ctx);

void graphics_draw_human(struct graphics_ctx *ctx, double x, double y, ALLEGRO_COLOR *colour);

void graphics_end(struct graphics_ctx *ctx);

#endif
