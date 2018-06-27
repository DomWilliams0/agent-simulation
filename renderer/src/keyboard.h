#ifndef SIM_KEYBOARD_H
#define SIM_KEYBOARD_H

#include <stdbool.h>

#include "graphics.h"
#include "util/util.h"

struct camera_movement;

MOD_DECLARE(keyboard, {
	struct camera_movement camera;
});

bool keyboard_handle_camera_key(struct keyboard *self, bool is_down, int key);

void keyboard_get_camera_changes(struct keyboard *self, struct camera_movement *out);

#endif
