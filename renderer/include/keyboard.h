#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "util/util.h"
#include "util/bool.h"

struct camera_movement;

MODULE_DECLARE(struct keyboard_ctx, keyboard_init, keyboard_destroy);

BOOL keyboard_handle_camera_key(struct keyboard_ctx *ctx, BOOL is_down, int key);

void keyboard_get_camera_changes(struct keyboard_ctx *ctx, struct camera_movement *out);

#endif
