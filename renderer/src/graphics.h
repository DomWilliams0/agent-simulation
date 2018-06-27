#ifndef SIM_GRAPHICS_H
#define SIM_GRAPHICS_H

#include <entity/components.h>
#include "util/util.h"

#define WINDOW_TITLE  "Simulator"
#define WINDOW_WIDTH  (600)
#define WINDOW_HEIGHT (400)

struct camera_movement
{
	float move_hor;
	float move_ver;
	float zoom;
};

struct world;
struct ecs_comp_human;

MOD_FWD_DECLARE(graphics) // implementation specific

void graphics_start(struct graphics *self);

void graphics_draw_world(struct world *world);

void graphics_draw_human(cpVect pos, struct ecs_comp_human *human);

void graphics_end(struct graphics *self);

void graphics_resize(struct graphics *self, int w, int h);

void graphics_update_camera(struct graphics *self, struct camera_movement changes);

#endif
